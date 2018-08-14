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
    long long SaveTile(long long parentAreaId, std::string uuid, const Area& tileArea);
    long long SaveTileFile(long long tileId, std::string filename, std::string layerName, int year);
    long long SaveMetadata(std::string layerName, long long tileId, const Feature& buildingFeature);

    static unique_ptr<DatabaseWrapper> DatabaseWrapperFor(std::string vectorFilename);

private:
    long long SaveFeature(std::string tableName, std::function<void(Feature&)> saveFunction);
    long long SaveFeature(std::string tableName, const Feature& feature);

private:
    std::shared_ptr<VectorFile> _vectorFile;
};

#endif // DATABASEWRAPPER_H
