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

int DatabaseWrapper::SaveAreaOfInterest(const Area &areaOfInterest)
{
    return SaveFeature("areaofinterest", [&](Feature& feature)
    {
        feature.SetField("description", areaOfInterest.Description());
        feature.SetGeometry<Polygon>(areaOfInterest);
    });
}

int DatabaseWrapper::SaveTile(int parentAreaId, std::string uuid, const Area &tileArea)
{
    return SaveFeature("tiles", [&](Feature& feature)
    {
        feature.SetField("area_id", parentAreaId);
        feature.SetField("uuid", uuid);
        feature.SetGeometry<Polygon>(tileArea);
    });
}

int DatabaseWrapper::SaveTileFile(int tileId, std::string filename, std::string layerName, int year)
{
    return SaveFeature("tile_files", [&](Feature& feature)
    {
        feature.SetField("tile_id", tileId);
        feature.SetField("filename", filename);
        feature.SetField("layername", layerName);
        feature.SetField("year", year);
    });
}

shared_ptr<DatabaseWrapper> DatabaseWrapper::DatabaseWrapperFor(std::string vectorFilename)
{
    if (vectorFilename.empty())
        return nullptr;
    auto vectorFile = Utils::LoadVectorFile(vectorFilename, VectorFile::OpenMode::Update);
    return make_shared<DatabaseWrapper>(vectorFile);
}

int DatabaseWrapper::SaveFeature(std::string tableName, std::function<void (Feature &)> saveFunction)
{
    auto layer = _vectorFile->operator[](tableName.c_str());
    auto feature = layer->NewFeature();
    saveFunction(feature);
    layer->AddFeature(feature);
    layer->Save();
    return feature.FeatureId();
}
