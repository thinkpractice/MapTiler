#include "TileProducerStep.h"
#include "Utils.h"
#include "StepData.h"
#include <iostream>
#include <memory>

TileProducerStep::TileProducerStep(const TileGrid& tileGrid)
                :   ProcessingStep(PreProcessing),
                    _tileGrid(tileGrid)
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
    //Notify process is ended
    OutQueue()->enqueue(nullptr);
}
