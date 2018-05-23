#include "ProcessingPipeline.h"

ProcessingPipeline::ProcessingPipeline()
{
}

ProcessingPipeline::~ProcessingPipeline()
{
    for (auto& thread : _threads)
        thread.join();
}

void ProcessingPipeline::StartProcessing()
{
    for (auto& step : _processingSteps)
        _threads.push_back(thread([&]
                    {
                        step->Run();
                    })
                );
}

void ProcessingPipeline::AddProcessingStep(ProcessingStep* step)
{
    int previousStepIndex = (int)_processingSteps.size() - 1;

    _processingSteps.push_back(shared_ptr<ProcessingStep>(step));

    if (previousStepIndex < 0)
    {
        shared_ptr<SafeQueue<GeoTile*>> inQueue(new SafeQueue<GeoTile*>());
        _queues.push_back(inQueue);
    }

    shared_ptr<SafeQueue<GeoTile*>> outQueue(new SafeQueue<GeoTile*>());
    _queues.push_back(outQueue);

    shared_ptr<SafeQueue<GeoTile*>> inQueue = _queues[previousStepIndex + 1];
    step->InitQueues(inQueue, outQueue);
}

