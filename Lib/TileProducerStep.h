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
    TileProducerStep(std::shared_ptr<GeoMap> map, const Rect& rectToProcess, const Area& areaToProcess, int tileWidth, int tileHeight, std::string persistenceUrl = "", bool createNewTilesIfAlreadyAvailable = false);
    virtual ~TileProducerStep();

    virtual void Run();

    void CreateStepData();
    std::string PersistenceUrl();
    Area AreaOfInterest();
    Rect RectToProcess();

protected:
    bool TilesInDatabase(std::shared_ptr<DatabaseWrapper> databasePersistence, Area areaToProcess);
    void CreateTile(std::shared_ptr<DatabaseWrapper> databasePersistence, long long areaId, const Rect& tileRect, int totalNumberOfTiles);
    virtual void CreateTiles(std::shared_ptr<DatabaseWrapper> databasePersistence) = 0;
    void LoadTiles(std::shared_ptr<DatabaseWrapper> databasePersistence, Area areaToProcess);

private:
    std::shared_ptr<GeoMap> _map;
    Area _area;
    Rect _rectToProcess;
    std::string _persistenceUrl;
    bool _createNewTilesIfAlreadyAvailable;
};

#endif /* TILEPRODUCERSTEP_H */
