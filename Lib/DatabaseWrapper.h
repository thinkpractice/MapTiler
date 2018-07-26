#ifndef DATABASEWRAPPER_H
#define DATABASEWRAPPER_H

#include <memory>
#include <string>
#include "GeoMap.h"
#include "Area.h"

class DatabaseWrapper
{
public:
    DatabaseWrapper(std::shared_ptr<GeoMap> geoMap);
    virtual ~DatabaseWrapper();

    int SaveAreaOfInterest(const Area& areaOfInterest);
    int SaveTile(int parentAreaId, std::string uuid, const Area& tileArea);
    int SaveTileFile(int tileId, std::string filename, std::string layerName, int year);

private:
    std::shared_ptr<GeoMap> _map;
};

#endif // DATABASEWRAPPER_H
