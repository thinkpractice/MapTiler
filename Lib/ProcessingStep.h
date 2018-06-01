#ifndef PROCESSINGSTEP_H
#define PROCESSINGSTEP_H

#include <memory>
#include "Rect.h"
#include "GeoTile.h"
#include "SafeQueue.h"

using namespace std;

class ProcessingStep
{
    public:
        enum ProcessingType { PreProcessing, PostProcessing };

        using TileType = shared_ptr<GeoTile>;
        using QueueType = SafeQueue<TileType>;
    public:
        ProcessingStep(ProcessingType type);
        virtual ~ProcessingStep();

        ProcessingType Type();
        virtual void Run() = 0;
       
        void InitQueues(shared_ptr<QueueType> inQueue, shared_ptr<QueueType> outQueue);
        shared_ptr<QueueType> InQueue() { return _inQueue; };
        shared_ptr<QueueType> OutQueue() { return _outQueue; };

    private:
        ProcessingType _type;
        shared_ptr<QueueType> _inQueue;
        shared_ptr<QueueType> _outQueue;
};

#endif /* PROCESSINGSTEP_H */
