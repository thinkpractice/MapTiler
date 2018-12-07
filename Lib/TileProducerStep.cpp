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
                    _tileGrid(rectToProcess, areaToProcess, tileWidth, tileHeight),
                    _map(map),
                    _persistenceUrl(persistenceUrl),
                    _createNewTilesIfAlreadyAvailable(createNewTilesIfAlreadyAvailable)
{
}

TileProducerStep::~TileProducerStep()
{
}

bool TileProducerStep::TilesInDatabase(std::shared_ptr<DatabaseWrapper> databasePersistence, Area areaToProcess)
{
    if (databasePersistence && databasePersistence->GetAreaFor(areaToProcess.Description()))
        return true;
    return false;
}

void TileProducerStep::CreateTile(std::shared_ptr<DatabaseWrapper> databasePersistence, long long areaId, const Rect& tileRect)
{
    try
    {
        Area area = _map->AreaForRect(tileRect);
        auto stepData = make_unique<StepData>(tileRect, area);
        stepData->SetAreaId(areaId);

        long long tileId = databasePersistence ? databasePersistence->SaveTile(areaId, stepData->UniqueId(), area) : 0;
        stepData->SetTileId(tileId);

        StepData::SetNumberOfTiles(_tileGrid.NumberOfTiles());

        OutQueue()->enqueue(std::move(stepData));
    }
    catch (...)
    {
       std::cerr << "Error creating tiles." << std::endl;
    }
}

void TileProducerStep::CreateTiles(std::shared_ptr<DatabaseWrapper> databasePersistence)
{
    std::cout << "Creating new tiles for area: " << _tileGrid.GridArea().Description() << endl;
    long long areaId = databasePersistence ? databasePersistence->SaveAreaOfInterest(_tileGrid.GridArea()) : 0;
    for (auto& tileRect : _tileGrid)
    {
        CreateTile(databasePersistence, areaId, tileRect);
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

    std::cout << "Loading tiles from database for area: " << _tileGrid.GridArea().Description() << endl;
    StepData::SetNumberOfTiles(tilesForArea.size());
    try
    {
        for (auto& tileFeature : tilesForArea)
        {
            long long areaId = std::strtoll(tileFeature["area_id"].Value().c_str(), nullptr, 10);
            long long tileId = std::strtoll(tileFeature["tile_id"].Value().c_str(), nullptr, 10);

            Area area = tileFeature.GetGeometry();
            Rect tileRect = _map->RectForArea(area);

            auto stepData = make_unique<StepData>(tileRect, area);
            stepData->SetAreaId(areaId);
            stepData->SetTileId(tileId);

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
    cout << "TileGrid dimensions=(" << _tileGrid.WidthInTiles() << "," << _tileGrid.HeightInTiles() << ")" << endl;
    cout << "Pixel dimensions" << _tileGrid.PixelDimensions().Left() << "," << _tileGrid.PixelDimensions().Top() << "," << _tileGrid.PixelDimensions().Width() << "," << _tileGrid.PixelDimensions().Height() << endl;
    cout << "Tile Dimensions=(" << _tileGrid.TileWidth() << "," << _tileGrid.TileHeight() << ")" << endl;

    std::shared_ptr<DatabaseWrapper> databasePersistence = DatabaseWrapper::DatabaseWrapperFor(_persistenceUrl);
    if (_createNewTilesIfAlreadyAvailable || !TilesInDatabase(databasePersistence, _tileGrid.GridArea()))
        CreateTiles(databasePersistence);
    else
        LoadTiles(databasePersistence, _tileGrid.GridArea());

    DoneProcessing();
}


