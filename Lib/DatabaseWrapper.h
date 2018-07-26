#ifndef DATABASEWRAPPER_H
#define DATABASEWRAPPER_H

#include <memory>
#include <functional>
#include <string>
#include "Area.h"
#include "Layer.h"
#include "Feature.h"

class DatabaseWrapper
{
public:
    DatabaseWrapper(std::shared_ptr<Layer> vectorFile);
    virtual ~DatabaseWrapper();

    int SaveAreaOfInterest(const Area& areaOfInterest);
    int SaveTile(int parentAreaId, std::string uuid, const Area& tileArea);
    int SaveTileFile(int tileId, std::string filename, std::string layerName, int year);

    static shared_ptr<DatabaseWrapper> DatabaseWrapperFor(std::string vectorFilename, std::string LayerName);

private:
    int SaveFeature(std::function<void(Feature&)> saveFunction);

private:
    std::shared_ptr<Layer> _layer;
};

#endif // DATABASEWRAPPER_H
