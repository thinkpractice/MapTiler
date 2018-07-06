#include "ProcessingStep.h"

ProcessingStep::ProcessingStep(ProcessingType type)
                    :   _type(type)
{
}

ProcessingStep::~ProcessingStep()
{
}

ProcessingStep::ProcessingType ProcessingStep::Type()
{
    return _type;
}

void ProcessingStep::InitQueues(shared_ptr<QueueType> inQueue, shared_ptr<QueueType> outQueue)
{
    _inQueue = inQueue;
    _outQueue = outQueue;
}

void ProcessingStep::DoneProcessing()
{
    OutQueue()->enqueue(nullptr);
}
