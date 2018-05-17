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
        for (auto& tileRect : _tileGrid)
        {
            auto result = threadPool.enqueue([&, tileRect]{
                        GeoMap* map = mapsPerThread.dequeue();
                        try
                        {
                            GeoTile* tile = map->GetTileForRect(tileRect);
                            tile->SetUniqueId(to_string(currentIndex));
                            //TODO progress messages
                            //TODO possible issue here with multiple threads accessing the same vector, move outside of threading code?
                            //TODO implement ProcessingState object with common variables? like the GeoTile being processed?
                            for (auto step : _preProcessingSteps)
                                step->ProcessTile(tile, tileRect);

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

void TileProcessor::AddProcessingStep(ProcessingStep* step)
{
    if (step->Type() == ProcessingStep::PreProcessing)
        _preProcessingSteps.push_back(shared_ptr<ProcessingStep>(step));
    if (step->Type() == ProcessingStep::PostProcessing)
        _postProcessingSteps.push_back(shared_ptr<ProcessingStep>(step));
}

