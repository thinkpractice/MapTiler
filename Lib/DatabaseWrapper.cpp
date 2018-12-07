#include "DatabaseWrapper.h"
#include "VectorFile.h"
#include "Utils.h"

DatabaseWrapper::DatabaseWrapper(std::shared_ptr<VectorFile> vectorFile)
                    :	_vectorFile(vectorFile)
{
}

DatabaseWrapper::~DatabaseWrapper()
{
}

long long DatabaseWrapper::SaveAreaOfInterest(const Area &areaOfInterest)
{
    return SaveFeature("areaofinterest", [&](Feature& feature)
    {
        feature.SetField("description", areaOfInterest.Description());
        feature.SetGeometry(areaOfInterest);
    });
}

long long DatabaseWrapper::SaveTile(long long parentAreaId, std::string uuid, const Area &tileArea)
{
    return SaveFeature("tiles", [&](Feature& feature)
    {
        feature.SetField("area_id", parentAreaId);
        feature.SetField("uuid", uuid);
        feature.SetGeometry(tileArea);
    });
}

long long DatabaseWrapper::SaveTileFile(long long tileId, std::string filename, std::string layerName, int year)
{
    return SaveFeature("tile_files", [&](Feature& feature)
    {
        feature.SetField("tile_id", tileId);
        feature.SetField("filename", filename);
        feature.SetField("layername", layerName);
        feature.SetField("year", year);
    });
}

long long DatabaseWrapper::SaveMetadata(std::string layerName, long long tileId, const Feature &buildingFeature)
{
    if (layerName.empty())
        return -1;

    long long metadataRelationId = SaveFeature(layerName, [&](Feature& feature)
    {
        feature.SetField("tile_id", tileId);
        auto featureId = buildingFeature.FeatureId();
        feature.SetField("metadata_id", featureId);
    });
    return metadataRelationId;
}

std::unique_ptr<Feature> DatabaseWrapper::GetAreaFor(std::string areaName)
{
    auto results = _vectorFile->ExecuteSql("select * from areaofinterest where description = '" + areaName + "'");
    if (results.size() > 0)
        return make_unique<Feature>(results[0]);
    return nullptr;
}

std::vector<Feature> DatabaseWrapper::GetTilesForArea(std::string areaName)
{
   return _vectorFile->ExecuteSql("select ti.* from areaofinterest ai "
                                  "inner join tiles as ti on ai.area_id = ti.area_id "
                                  "where description = '" + areaName + "'");
}

unique_ptr<DatabaseWrapper> DatabaseWrapper::DatabaseWrapperFor(std::string vectorFilename)
{
    if (vectorFilename.empty())
        return nullptr;
    auto vectorFile = Utils::LoadVectorFile(vectorFilename, VectorFile::OpenMode::Update);
    return make_unique<DatabaseWrapper>(vectorFile);
}

long long DatabaseWrapper::SaveFeature(std::string tableName, std::function<void (Feature &)> saveFunction)
{
    auto layer = _vectorFile->operator[](tableName.c_str());
    auto feature = layer->NewFeature();
    saveFunction(feature);
    return SaveFeature(tableName, feature);
}

long long DatabaseWrapper::SaveFeature(std::string tableName, const Feature &feature)
{
    auto layer = _vectorFile->operator[](tableName.c_str());
    layer->AddFeature(feature);
    layer->Save();
    return feature.FeatureId();
}
