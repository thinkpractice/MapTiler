#ifndef TILEPRODUCERSTEP_H
#define TILEPRODUCERSTEP_H

#include "ProcessingStep.h"
#include "TileGrid.h"

class TileProducerStep : public ProcessingStep
{
public:
    TileProducerStep(const TileGrid& tileGrid);
    virtual ~TileProducerStep();

    void Run();

private:
    TileGrid _tileGrid;
};

#endif /* TILEPRODUCERSTEP_H */
