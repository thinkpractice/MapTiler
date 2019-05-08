#include "TileProducerStep.h"
#include "Utils.h"
#include "StepData.h"
#include <iostream>
#include <memory>
#include <cstdlib>

TileProducerStep::TileProducerStep(std::string layerUrl, int layerIndex, const Area &area, int tileWidth, int tileHeight, std::string persistenceUrl)
                    :	TileProducerStep(Utils::LoadRasterMap(layerUrl, layerIndex), area, tileWidth, tileHeight, persistenceUrl)
{
}

TileProducerStep::TileProducerStep(std::shared_ptr<GeoMap> map, const Area &area, int tileWidth, int tileHeight, std::string persistenceUrl)
                    :	TileProducerStep(map, map->RectForArea(area), area, tileWidth, tileHeight, persistenceUrl)
{

}

TileProducerStep::TileProducerStep(std::shared_ptr<GeoMap> map, const Rect& rectToProcess, const Area& areaToProcess, int tileWidth, int tileHeight, std::string persistenceUrl, bool createNewTilesIfAlreadyAvailable)
                :   ProcessingStep(Source),
                    _map(map),
                    _area(areaToProcess),
                    _rectToProcess(rectToProcess),
                    _persistenceUrl(persistenceUrl),
                    _createNewTilesIfAlreadyAvailable(createNewTilesIfAlreadyAvailable)
{
}

TileProducerStep::~TileProducerStep()
{
}

std::string TileProducerStep::PersistenceUrl()
{
    return _persistenceUrl;
}

Area TileProducerStep::AreaOfInterest()
{
    return _area;
}

Rect TileProducerStep::RectToProcess()
{
    return _rectToProcess;
}

bool TileProducerStep::TilesInDatabase(std::shared_ptr<DatabaseWrapper> databasePersistence, Area areaToProcess)
{
    if (databasePersistence && databasePersistence->GetAreaFor(areaToProcess.Description()))
        return true;
    return false;
}

void TileProducerStep::CreateTile(std::shared_ptr<DatabaseWrapper> databasePersistence, long long areaId, const Rect& tileRect, int totalNumberOfTiles)
{
    try
    {
        Area area = _map->AreaForRect(tileRect);
        auto stepData = make_unique<StepData>(tileRect, area);
        stepData->SetAreaId(areaId);

        long long tileId = databasePersistence ? databasePersistence->SaveTile(areaId, stepData->UniqueId(), area) : 0;
        stepData->SetTileId(tileId);

        StepData::SetNumberOfTiles(totalNumberOfTiles);

        OutQueue()->enqueue(std::move(stepData));
    }
    catch (...)
    {
       std::cerr << "Error creating tiles." << std::endl;
    }
}

void TileProducerStep::LoadTiles(std::shared_ptr<DatabaseWrapper> databasePersistence, Area areaToProcess)
{
    std::vector<Feature> tilesForArea = databasePersistence ?
                databasePersistence->GetTilesForArea(areaToProcess.Description()) :
                std::vector<Feature>();

    if (tilesForArea.size() == 0)
    {
        std::cerr << "No tiles found in database for area: " << areaToProcess.Description() << std::endl;
        return;
    }

    std::cout << "Loading tiles from database for area: " << AreaOfInterest().Description() << endl;
    StepData::SetNumberOfTiles(tilesForArea.size());
    try
    {
        for (auto& tileFeature : tilesForArea)
        {
            long long areaId = std::strtoll(tileFeature["area_id"].Value().c_str(), nullptr, 10);
            long long tileId = std::strtoll(tileFeature["tile_id"].Value().c_str(), nullptr, 10);
            std::string uuid = tileFeature["uuid"].Value();

            Area area = tileFeature.GetGeometry();
            Rect tileRect = _map->RectForArea(area);

            auto stepData = make_unique<StepData>(tileRect, area);
            stepData->SetAreaId(areaId);
            stepData->SetTileId(tileId);
            stepData->SetUniqueId(uuid);

            OutQueue()->enqueue(std::move(stepData));
        }
    }
    catch (...)
    {
       std::cerr << "Error loading tiles from database." << std::endl;
    }
}

void TileProducerStep::Run()
{
    cout << "Map dimensions = (" << _map->WidthInPixels() << "," << _map->HeightInPixels() << ")" << endl;

    std::shared_ptr<DatabaseWrapper> databasePersistence = DatabaseWrapper::DatabaseWrapperFor(_persistenceUrl);
    if (_createNewTilesIfAlreadyAvailable || !TilesInDatabase(databasePersistence, AreaOfInterest()))
        CreateTiles(databasePersistence);
    else
        LoadTiles(databasePersistence, AreaOfInterest());

    DoneProcessing();
}


