#ifndef PROCESSINGPIPELINE_H
#define PROCESSINGPIPELINE_H

#include <vector>
#include <memory>
#include <thread>
#include "ProcessingStep.h"
#include "SafeQueue.h"
#include "GeoTile.h"

class ProcessingPipeline
{
public:
    ProcessingPipeline();
    virtual ~ProcessingPipeline();

    void StartProcessing();
    void AddProcessingStep(std::shared_ptr<ProcessingStep> step);

private:
    std::vector< std::shared_ptr<ProcessingStep::QueueType> > _queues;
    std::vector< std::shared_ptr<ProcessingStep> > _processingSteps;
    std::vector< std::thread > _threads;
};

#endif /* PROCESSINGPIPELINE_H */
