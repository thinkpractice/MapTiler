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

    long long SaveAreaOfInterest(const Area& areaOfInterest);
    long long SaveTile(int parentAreaId, std::string uuid, const Area& tileArea);
    long long SaveTileFile(int tileId, std::string filename, std::string layerName, int year);
    long long SaveBuilding(int tileId, const Feature& buildingFeature);

    static shared_ptr<DatabaseWrapper> DatabaseWrapperFor(std::string vectorFilename);

private:
    long long SaveFeature(std::string tableName, std::function<void(Feature&)> saveFunction);
    long long SaveFeature(std::string tableName, const Feature& feature);

private:
    std::shared_ptr<VectorFile> _vectorFile;
};

#endif // DATABASEWRAPPER_H
