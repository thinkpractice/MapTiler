#include "TileProducerStep.h"
#include "Utils.h"
#include "StepData.h"
#include "DatabaseWrapper.h"
#include <iostream>
#include <memory>

#include "CoordinateTransformation.h"

TileProducerStep::TileProducerStep(std::string layerUrl, int layerIndex, const Area &area, int tileWidth, int tileHeight, std::string persistenceUrl, std::string persistenceLayerName)
                    :	TileProducerStep(Utils::LoadRasterMap(layerUrl, layerIndex), area, tileWidth, tileHeight, persistenceUrl, persistenceLayerName)
{
}

TileProducerStep::TileProducerStep(std::shared_ptr<GeoMap> map, const Area &area, int tileWidth, int tileHeight, std::string persistenceUrl, std::string persistenceLayerName)
                    :	TileProducerStep(map, map->RectForArea(area), area, tileWidth, tileHeight, persistenceUrl, persistenceLayerName)
{

}

TileProducerStep::TileProducerStep(std::shared_ptr<GeoMap> map, const Rect& rectToProcess, const Area& areaToProcess, int tileWidth, int tileHeight, std::string persistenceUrl, std::string persistenceLayerName)
                :   ProcessingStep(Source),
					_map(map),
                    _tileGrid(rectToProcess, areaToProcess, tileWidth, tileHeight),
                    _persistenceUrl(persistenceUrl),
                    _persistenceLayerName(persistenceLayerName)
{
}

TileProducerStep::~TileProducerStep()
{
}

void TileProducerStep::Run()
{
    int numberOfTilesDownloaded = 0;
    cout << "Map dimensions = (" << _map->WidthInPixels() << "," << _map->HeightInPixels() << ")" << endl;
    cout << "TileGrid dimensions=(" << _tileGrid.WidthInTiles() << "," << _tileGrid.HeightInTiles() << ")" << endl;
    cout << "Tile Dimensions=(" << _tileGrid.TileWidth() << "," << _tileGrid.TileHeight() << ")" << endl;

    shared_ptr<DatabaseWrapper> databasePersistence = DatabaseWrapper::DatabaseWrapperFor(_persistenceUrl);
    int areaId = databasePersistence ? databasePersistence->SaveAreaOfInterest(_tileGrid.GridArea()) : 0;

    Area mapArea = _map->GetMapArea();
    for (auto& tileRect : _tileGrid)
    {
        try
        {
            Area area = _map->AreaForRect(tileRect);
            auto stepData = make_shared<StepData>(tileRect, area);
            stepData->SetAreaId(areaId);

            int tileId = databasePersistence ? databasePersistence->SaveTile(areaId, stepData->UniqueId(), area) : 0;
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
