#include "DatabaseWrapper.h"
#include "VectorFile.h"
#include "Utils.h"

DatabaseWrapper::DatabaseWrapper(std::shared_ptr<Layer> layer)
                    :	_layer(layer)
{
}

int DatabaseWrapper::SaveAreaOfInterest(const Area &areaOfInterest)
{
    return SaveFeature([&](Feature& feature)
    {
        feature.SetField("description", areaOfInterest.Description());
        feature.SetGeometry(areaOfInterest);
    });
}

int DatabaseWrapper::SaveTile(int parentAreaId, std::string uuid, const Area &tileArea)
{
    return SaveFeature([&](Feature& feature)
    {
        feature.SetField("area_id", parentAreaId);
        feature.SetField("uuid", uuid);
        feature.SetGeometry(tileArea);
    });
}

int DatabaseWrapper::SaveTileFile(int tileId, std::string filename, std::string layerName, int year)
{
    return SaveFeature([&](Feature& feature)
    {
        feature.SetField("tile_id", tileId);
        feature.SetField("filename", filename);
        feature.SetField("layername", layerName);
        feature.SetField("year", year);
    });
}

DatabaseWrapper DatabaseWrapper::DatabaseWrapperFor(std::string vectorFilename, std::string layerName)
{
    auto vectorFile = Utils::LoadVectorFile(vectorFilename);
    auto layer = vectorFile->operator[](layerName.c_str());
    return DatabaseWrapper(layer);
}

int DatabaseWrapper::SaveFeature(std::function<void (Feature &)> saveFunction)
{
    auto feature = _layer->NewFeature();
    saveFunction(feature);
    _layer->AddFeature(feature);
    _layer->Save();
    return feature.FeatureId();
}
