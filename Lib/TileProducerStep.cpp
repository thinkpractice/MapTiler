#include "TileProducerStep.h"
#include <iostream>

TileProducerStep::TileProducerStep(GeoMap* map, const TileGrid& tileGrid)
                :   ProcessingStep(PreProcessing),
                    _map(map),
                    _tileGrid(tileGrid)
{
}

TileProducerStep::~TileProducerStep()
{
    delete _map;
}

void TileProducerStep::Run()
{
    int numberOfTilesDownloaded = 0;
    for (auto& tileRect : _tileGrid)
    {
        try
        {
            GeoTile* tile = _map->GetTileForRect(tileRect);
            //tile->SetUniqueId(to_string(currentIndex));
            //TODO implement ProcessingState object with common variables? like the GeoTile being processed?

            OutQueue()->enqueue(tile);
            numberOfTilesDownloaded++;

            if (numberOfTilesDownloaded % 100 == 0)
                cout << "Downloaded " << to_string(numberOfTilesDownloaded) << " out of " << to_string(_tileGrid.NumberOfTiles()) << endl;
        }
        catch (...)
        {
        }
    }
}
