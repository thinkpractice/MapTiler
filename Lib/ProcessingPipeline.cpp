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

void ProcessingPipeline::AddProcessingStep(shared_ptr<ProcessingStep> step)
{
    int previousStepIndex = (int)_processingSteps.size() - 1;

    _processingSteps.push_back(step);

    if (previousStepIndex < 0)
    {
        shared_ptr<SafeQueue<shared_ptr<GeoTile>>> inQueue(new SafeQueue<shared_ptr<GeoTile>>());
        _queues.push_back(inQueue);
    }

    shared_ptr<SafeQueue<shared_ptr<GeoTile>>> outQueue(new SafeQueue<shared_ptr<GeoTile>>());
    _queues.push_back(outQueue);

    shared_ptr<SafeQueue<shared_ptr<GeoTile>>> inQueue = _queues[previousStepIndex + 1];
    step->InitQueues(inQueue, outQueue);
}
