#ifndef PROCESSINGSTEP_H
#define PROCESSINGSTEP_H

#include "Rect.h"
#include "GeoTile.h"

class ProcessingStep
{
    public:
        enum ProcessingType { PreProcessing, PostProcessing };

    public:
        ProcessingStep(ProcessingType type);
        virtual ~ProcessingStep();

        ProcessingType Type();
        virtual void ProcessTile(GeoTile* tile, Rect tileRect) = 0;

    private:
        ProcessingType _type;
};

#endif /* PROCESSINGSTEP_H */
