#ifndef TILEPRODUCERSTEP_H
#define TILEPRODUCERSTEP_H

#include "ProcessingStep.h"
#include "TileGrid.h"
#include "Rect.h"
#include "Area.h"

using namespace std;

class TileProducerStep : public ProcessingStep
{
public:
    TileProducerStep(const Rect& rectToProcess, const Area& areaToProcess, int tileWidth, int tileHeight);
    virtual ~TileProducerStep();

    void Run();

private:
    TileGrid _tileGrid;
};

#endif /* TILEPRODUCERSTEP_H */
