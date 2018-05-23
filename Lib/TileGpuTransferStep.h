#ifndef TILEGPUTRANSFERSTEP_H
#define TILEGPUTRANSFERSTEP_H

#include "ProcessingStep.h"

class TileGpuTransferStep : public ProcessingStep
{
public:
    TileGpuTransferStep();
    virtual ~TileGpuTransferStep();

    void Run();
};

#endif /* TILEGPUTRANSFERSTEP_H */
