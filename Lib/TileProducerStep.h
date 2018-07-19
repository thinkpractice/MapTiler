#ifndef TILEPRODUCERSTEP_H
#define TILEPRODUCERSTEP_H

#include <memory>
#include "ProcessingStep.h"
#include "TileGrid.h"
#include "Rect.h"
#include "Area.h"
#include "GeoMap.h"

class TileProducerStep : public ProcessingStep
{
public:
    TileProducerStep(std::shared_ptr<GeoMap> map, const Area& area, int tileWidth, int tileHeight);
    TileProducerStep(std::shared_ptr<GeoMap> map, const Rect& rectToProcess, const Area& areaToProcess, int tileWidth, int tileHeight);
    virtual ~TileProducerStep();

    void Run();

private:
    TileGrid _tileGrid;
	std::shared_ptr<GeoMap> _map;
};

#endif /* TILEPRODUCERSTEP_H */
