#include "TileProcessor.h"
#include "SafeQueue.h"
#include "ThreadPool.h"
#include <atomic>
#include <string>

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
        
    }

    while (!mapsPerThread.Empty())
    {
        GeoMap* map = mapsPerThread.dequeue();
        delete map;
    }
}

void TileProcessor::AddProcessingStep(ProcessingStep* step)
{
    _processingSteps.push_back(shared_ptr<ProcessingStep>(step));

    int stepIndex = (int)_processingSteps.size() - 1;
    if (stepIndex < 0)
    {
        shared_ptr<SafeQueue<GeoTile*>> inQueue(new SafeQueue<GeoTile*>());
        _queues.push_back(inQueue);
    }

    shared_ptr<SafeQueue<GeoTile*>> outQueue(new SafeQueue<GeoTile*>());
    _queues.push_back(outQueue);

    shared_ptr<SafeQueue<GeoTile*>> inQueue = _queues[stepIndex + 1];
    step->InitQueues(inQueue, outQueue);
}

