#ifndef PROCESSINGPIPELINE_H
#define PROCESSINGPIPELINE_H

#include <vector>
#include <memory>
#include <thread>
#include "ProcessingStep.h"
#include "SafeQueue.h"
#include "GeoTile.h"

using namespace std;

class ProcessingPipeline
{
public:
    ProcessingPipeline();
    virtual ~ProcessingPipeline();

    void StartProcessing();
    void AddProcessingStep(ProcessingStep* step);

private:
        vector< shared_ptr<SafeQueue<GeoTile*>> > _queues;
        vector< shared_ptr<ProcessingStep> > _processingSteps;
        vector<thread> _threads;
};

#endif /* PROCESSINGPIPELINE_H */
