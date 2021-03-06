#ifndef TILEPRODUCERSTEP_H
#define TILEPRODUCERSTEP_H

#include <memory>
#include "ProcessingStep.h"
#include "TileGrid.h"
#include "Rect.h"
#include "Area.h"
#include "GeoMap.h"
#include "DatabaseWrapper.h"

class TileProducerStep : public ProcessingStep
{
public:
    TileProducerStep(std::string layerUrl, int layerIndex, const Area& area, int tileWidth, int tileHeight, std::string persistenceUrl = "");
    TileProducerStep(std::shared_ptr<GeoMap> map, const Area &area, int tileWidth, int tileHeight, std::string persistenceUrl = "");
    TileProducerStep(std::shared_ptr<GeoMap> map, const Rect& rectToProcess, const Area& areaToProcess, int tileWidth, int tileHeight, std::string persistenceUrl = "");
    virtual ~TileProducerStep();

    void Run();

    void CreateStepData();

private:
    void CreateTile(std::shared_ptr<DatabaseWrapper> databasePersistence, long long areaId, const Rect& tileRect);

private:
    TileGrid _tileGrid;
	std::shared_ptr<GeoMap> _map;
    std::string _persistenceUrl;
};

#endif /* TILEPRODUCERSTEP_H */
