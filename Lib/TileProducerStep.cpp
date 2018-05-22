#include "TileProducerStep.h"

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
    for (auto& tileRect : _tileGrid)
    {
       
            try
            {
                GeoTile* tile = _map->GetTileForRect(tileRect);
                //tile->SetUniqueId(to_string(currentIndex));
                //TODO progress messages
                //TODO implement ProcessingState object with common variables? like the GeoTile being processed?

                OutQueue()->enqueue(tile);
            }
            catch (...)
            {
            }
    }
}
