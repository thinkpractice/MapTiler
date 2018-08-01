#ifndef DATABASEWRAPPER_H
#define DATABASEWRAPPER_H

#include <memory>
#include <functional>
#include <string>
#include "Area.h"
#include "VectorFile.h"
#include "Feature.h"

class DatabaseWrapper
{
public:
    DatabaseWrapper(std::shared_ptr<VectorFile> vectorFile);
    virtual ~DatabaseWrapper();

    int SaveAreaOfInterest(const Area& areaOfInterest);
    int SaveTile(int parentAreaId, std::string uuid, const Area& tileArea);
    int SaveTileFile(int tileId, std::string filename, std::string layerName, int year);
    int SaveBuilding(int tileId, const Feature& buildingFeature);

    static shared_ptr<DatabaseWrapper> DatabaseWrapperFor(std::string vectorFilename);

private:
    int SaveFeature(std::string tableName, std::function<void(Feature&)> saveFunction);
    int SaveFeature(std::string tableName, const Feature& feature);

private:
    std::shared_ptr<VectorFile> _vectorFile;
};

#endif // DATABASEWRAPPER_H
