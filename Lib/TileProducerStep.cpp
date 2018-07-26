#include "TileProducerStep.h"
#include "Utils.h"
#include "StepData.h"
#include <iostream>
#include <memory>

TileProducerStep::TileProducerStep(std::string layerUrl, int layerIndex, const Area &area, int tileWidth, int tileHeight)
                    :	TileProducerStep(Utils::LoadRasterMap(layerUrl, layerIndex), area, tileWidth, tileHeight)
{
}

TileProducerStep::TileProducerStep(std::shared_ptr<GeoMap> map, const Area &area, int tileWidth, int tileHeight)
                    :	TileProducerStep(map, map->RectForArea(area), area, tileWidth, tileHeight)
{

}

TileProducerStep::TileProducerStep(std::shared_ptr<GeoMap> map, const Rect& rectToProcess, const Area& areaToProcess, int tileWidth, int tileHeight)
                :   ProcessingStep(Source),
					_map(map),
                    _tileGrid(rectToProcess, areaToProcess, tileWidth, tileHeight)
{
}

TileProducerStep::~TileProducerStep()
{
}

shared_ptr<StepData> TileProducerStep::CreateStepData(const Rect& tileRect)
{
    Area area = _map->AreaForRect(tileRect);
    return make_shared<StepData>(tileRect, area);
}

void TileProducerStep::Run()
{
    int numberOfTilesDownloaded = 0;
    cout << "Map dimensions = (" << _map->WidthInPixels() << "," << _map->HeightInPixels() << endl;
    cout << "TileGrid dimensions=(" << _tileGrid.WidthInTiles() << "," << _tileGrid.HeightInTiles() << ")" << endl;
    cout << "Tile Dimensions=(" << _tileGrid.TileWidth() << "," << _tileGrid.TileHeight() << ")" << endl;
    for (auto& tileRect : _tileGrid)
    {
        try
        {
            auto stepData = CreateStepData(tileRect);
			StepData::SetNumberOfTiles(_tileGrid.NumberOfTiles());
			
            OutQueue()->enqueue(stepData);
        }
        catch (...)
        {
        }
    }
    DoneProcessing();
}
