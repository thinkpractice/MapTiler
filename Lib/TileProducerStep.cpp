#include "TileProducerStep.h"
#include "Utils.h"
#include "StepData.h"
#include <iostream>
#include <memory>

TileProducerStep::TileProducerStep(std::shared_ptr<GeoMap> map, const Rect& rectToProcess, const Area& areaToProcess, int tileWidth, int tileHeight)
                :   ProcessingStep(PreProcessing),
					_map(map),
                    _tileGrid(rectToProcess, areaToProcess, tileWidth, tileHeight)
{
}

TileProducerStep::~TileProducerStep()
{
}

void TileProducerStep::Run()
{
    int numberOfTilesDownloaded = 0;
    cout << "TileGrid dimensions=(" << _tileGrid.WidthInTiles() << "," << _tileGrid.HeightInTiles() << ")" << endl;
    cout << "Tile Dimensions=(" << _tileGrid.TileWidth() << "," << _tileGrid.TileHeight() << ")" << endl;
    for (auto& tileRect : _tileGrid)
    {
        try
        {
			Area area = _map->AreaForRect(tileRect);
			auto stepData = make_shared<StepData>(tileRect, area);
			
			StepData::SetNumberOfTiles(_tileGrid.NumberOfTiles());
			
            OutQueue()->enqueue(stepData);
        }
        catch (...)
        {
        }
    }
    DoneProcessing();
}
