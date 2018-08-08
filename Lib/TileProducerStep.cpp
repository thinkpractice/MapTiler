#include "TileProducerStep.h"
#include "Utils.h"
#include "StepData.h"
#include "DatabaseWrapper.h"
#include <iostream>
#include <memory>

TileProducerStep::TileProducerStep(std::string layerUrl, int layerIndex, const Area &area, int tileWidth, int tileHeight, std::string persistenceUrl)
                    :	TileProducerStep(Utils::LoadRasterMap(layerUrl, layerIndex), area, tileWidth, tileHeight, persistenceUrl)
{
}

TileProducerStep::TileProducerStep(std::shared_ptr<GeoMap> map, const Area &area, int tileWidth, int tileHeight, std::string persistenceUrl)
                    :	TileProducerStep(map, map->RectForArea(area), area, tileWidth, tileHeight, persistenceUrl)
{

}

TileProducerStep::TileProducerStep(std::shared_ptr<GeoMap> map, const Rect& rectToProcess, const Area& areaToProcess, int tileWidth, int tileHeight, std::string persistenceUrl)
                :   ProcessingStep(Source),
                    _tileGrid(rectToProcess, areaToProcess, tileWidth, tileHeight),
                    _map(map),
                    _persistenceUrl(persistenceUrl)
{
}

TileProducerStep::~TileProducerStep()
{
}

void TileProducerStep::Run()
{
    cout << "Map dimensions = (" << _map->WidthInPixels() << "," << _map->HeightInPixels() << ")" << endl;
    cout << "TileGrid dimensions=(" << _tileGrid.WidthInTiles() << "," << _tileGrid.HeightInTiles() << ")" << endl;
    cout << "Pixel dimensions" << _tileGrid.PixelDimensions().Left() << "," << _tileGrid.PixelDimensions().Top() << "," << _tileGrid.PixelDimensions().Width() << "," << _tileGrid.PixelDimensions().Height() << endl;
    cout << "Tile Dimensions=(" << _tileGrid.TileWidth() << "," << _tileGrid.TileHeight() << ")" << endl;

    shared_ptr<DatabaseWrapper> databasePersistence = DatabaseWrapper::DatabaseWrapperFor(_persistenceUrl);
    long long areaId = databasePersistence ? databasePersistence->SaveAreaOfInterest(_tileGrid.GridArea()) : 0;

    Area mapArea = _map->GetMapArea();
    for (auto& tileRect : _tileGrid)
    {
        try
        {
            Area area = _map->AreaForRect(tileRect);
            auto stepData = make_shared<StepData>(tileRect, area);
            stepData->SetAreaId(areaId);

            long long tileId = databasePersistence ? databasePersistence->SaveTile(areaId, stepData->UniqueId(), area) : 0;
            stepData->SetTileId(tileId);

			StepData::SetNumberOfTiles(_tileGrid.NumberOfTiles());
			
            OutQueue()->enqueue(stepData);
        }
        catch (...)
        {
        }
    }
    DoneProcessing();
}
