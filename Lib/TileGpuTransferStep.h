#ifndef TILEGPUTRANSFERSTEP_H
#define TILEGPUTRANSFERSTEP_H

#include "ProcessingStep.h"
#include "VectorFile.h"

class TileGpuTransferStep : public ProcessingStep
{
public:
    TileGpuTransferStep(shared_ptr<VectorFile> vectorFile, int layerIndex);
    virtual ~TileGpuTransferStep();

    void Run();

private:
    shared_ptr<VectorFile> _vectorFile;
    int _layerIndex;
};

#endif /* TILEGPUTRANSFERSTEP_H */
