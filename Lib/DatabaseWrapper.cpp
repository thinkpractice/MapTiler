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

int DatabaseWrapper::SaveBuilding(int tileId, const Feature &buildingFeature)
{
    int buildingId = SaveFeature("buildings", [&](Feature feature)
    {
        feature.SetField("identifier", feature["identificatie"]);
        feature.SetField("year_build", feature["bouwjaar"]);
        feature.SetField("status", feature["status"]);
        feature.SetField("purpose", feature["gebruiksdoel"]);
        feature.SetField("area_min", feature["oppervlakte_min"]);
        feature.SetField("area_max", feature["oppervlakte_max"]);
        feature.SetField("number_of_residencies", feature["aantal_verblijfsobjecten"]);
        feature.SetField("update_date", feature["actualiteitsdatum"]);
        feature.SetGeometry<MultiPolygon>(feature.GetGeometry());
    });

    SaveFeature("tile_buildings", [&](Feature& feature)
    {
        feature.SetField("tile_id", tileId);
        feature.SetField("building_id", buildingId);
    });
    return buildingId;
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
    return SaveFeature(tableName, feature);
}

int DatabaseWrapper::SaveFeature(std::string tableName, const Feature &feature)
{
    auto layer = _vectorFile->operator[](tableName.c_str());
    layer->AddFeature(feature);
    layer->Save();
    return feature.FeatureId();
}
