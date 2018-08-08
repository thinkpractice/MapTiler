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

void ProcessingPipeline::AddProcessingStep(std::unique_ptr<ProcessingStep> step)
{
    int previousStepIndex = static_cast<int>(_processingSteps.size() - 1);
    if (previousStepIndex < 0)
    {
        shared_ptr<ProcessingStep::QueueType> inQueue(new ProcessingStep::QueueType());
        _queues.push_back(inQueue);
    }

    //TODO does this cause a retainment cycle?
    shared_ptr<ProcessingStep::QueueType> outQueue(new ProcessingStep::QueueType());
    _queues.push_back(outQueue);

    shared_ptr<ProcessingStep::QueueType> inQueue = _queues[previousStepIndex + 1];
    step->InitQueues(inQueue, outQueue);

    _processingSteps.push_back(std::move(step));
}
