#ifndef GRIDPRODUCERSTEP_H
#define GRIDPRODUCERSTEP_H

#include "TileProducerStep.h"

class GridProducerStep : public TileProducerStep
{
public:
    GridProducerStep(std::string layerUrl, int layerIndex, const Area& area, int tileWidth, int tileHeight, std::string persistenceUrl = "");
    virtual ~GridProducerStep();

    virtual void CreateTiles(std::shared_ptr<DatabaseWrapper> databasePersistence);
    virtual void Run();

private:
    TileGrid _tileGrid;
};

#endif // GRIDPRODUCERSTEP_H
