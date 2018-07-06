#include "TileProducerStep.h"
#include "Utils.h"
#include "StepData.h"
#include <iostream>
#include <memory>

TileProducerStep::TileProducerStep(const Rect& rectToProcess, const Area& areaToProcess, int tileWidth, int tileHeight)
                :   ProcessingStep(PreProcessing),
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
			auto stepData = make_shared<StepData>();
			stepData->SetNumberOfTiles(_tileGrid.NumberOfTiles());
			stepData->SetBoundingRect(tileRect);
            OutQueue()->enqueue(stepData);
        }
        catch (...)
        {
        }
    }
    DoneProcessing();
}
