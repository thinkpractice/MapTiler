#ifndef TILEFROMLOCATIONPRODUCER_H
#define TILEFROMLOCATIONPRODUCER_H

#include "TileProducerStep.h"

class TileFromLocationProducerStep : public TileProducerStep
{
public:
    TileFromLocationProducerStep(std::string mapLayerUrl, int mapLayerIndex, const Area& area, std::shared_ptr<VectorFile> vectorFile, std::string locationLayerName,
                                 int locationLayerIndex, int tileWidth, int tileHeight, std::string persistenceUrl = "");
    virtual ~TileFromLocationProducerStep();

    int LayerIndex();
    std::string LayerName();

    virtual void CreateTiles(std::shared_ptr<DatabaseWrapper> databasePersistence);
    shared_ptr<Layer> GetLayer();

private:
    std::shared_ptr<VectorFile> _vectorFile;
    std::string _locationLayerName;
    int _locationLayerIndex;
};

#endif // TILEFROMLOCATIONPRODUCER_H
