#include "TileProcessor.h"
#include "SafeQueue.h"
#include "ThreadPool.h"
#include <atomic>

TileProcessor::TileProcessor(GeoMap* mainRasterMap, int tileWidth, int tileHeight)
                :   TileProcessor(mainRasterMap, mainRasterMap->GetMapArea(), tileWidth, tileHeight)
{
}

TileProcessor::TileProcessor(GeoMap* mainRasterMap, const Area& areaToProcess, int tileWidth, int tileHeight)
                :   _mainRasterMap(mainRasterMap),
                    _areaToProcess(areaToProcess),
                    _tileGrid(_mainRasterMap->RectForArea(areaToProcess), areaToProcess, tileWidth, tileHeight)
{
}

TileProcessor::~TileProcessor()
{
}

void TileProcessor::StartProcessing()
{
    SafeQueue<GeoMap*> mapsPerThread;
    {
        auto t1 = std::chrono::high_resolution_clock::now();

        int numberOfThreads = 4;
        ThreadPool threadPool(numberOfThreads);
        
        mapsPerThread.enqueue(_mainRasterMap);

        for (int i = 0; i < numberOfThreads-1; i++)
        {
            mapsPerThread.enqueue(_mainRasterMap->Clone());
        }

        atomic<int> currentIndex (0);
        for (auto& tileRect : _tileGrid)
        {
            auto result = threadPool.enqueue([&, tileRect]{
                        GeoMap* map = mapsPerThread.dequeue();
                        try
                        {
                            GeoTile* tile = map->GetTileForRect(tileRect);

                            currentIndex += 1;
                        }
                        catch (...)
                        {
                        }
                        mapsPerThread.enqueue(map);
                    });
        }
    }

    while (!mapsPerThread.Empty())
    {
        GeoMap* map = mapsPerThread.dequeue();
        delete map;
    }
}

void TileProcessor::AddProcessingStep()
{
}
