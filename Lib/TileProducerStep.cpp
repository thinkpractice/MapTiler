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
    cout << "TileGrid dimensions=(" << _tileGrid.WidthInTiles() << "," << _tileGrid.HeightInTiles() << ")" << endl;
    cout << "Tile Dimensions=(" << _tileGrid.TileWidth() << "," << _tileGrid.TileHeight() << endl;
    for (auto& tileRect : _tileGrid)
    {
        try
        {
            cout << "downloading tile (" << tileRect.Left() << "," << tileRect.Top() << "," << tileRect.Right() << "," << tileRect.Bottom() << ")" << endl;
            GeoTile* tile = _map->GetTileForRect(tileRect);
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
