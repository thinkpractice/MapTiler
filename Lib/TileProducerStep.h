#ifndef TILEPRODUCERSTEP_H
#define TILEPRODUCERSTEP_H

#include "ProcessingStep.h"
#include "GeoMap.h"
#include "TileGrid.h"

class TileProducerStep : public ProcessingStep
{
public:
    TileProducerStep(GeoMap* map, const TileGrid& tileGrid);
    virtual ~TileProducerStep();

    void Run();

private:
    GeoMap* _map;
    TileGrid _tileGrid;
};

#endif /* TILEPRODUCERSTEP_H */
