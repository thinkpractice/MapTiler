#ifndef JSONSETTINGS_H
#define JSONSETTINGS_H

#include <fstream>
#include "json.hpp"
#include "Point.h"
#include "Area.h"
#include "Settings.h"
#include "StepSettings.h"

using json = nlohmann::json;

class JsonSettings
{
public:
    JsonSettings(std::string jsonString)
    {
        _jsonSettings = json::parse(jsonString);
    }

    template<class T> T GetValue(std::string name)
    {
        return _jsonSettings.at(name).get<T>();
    }

    static JsonSettings LoadFromFile(std::string filename)
    {
        std::ifstream readFileStream(filename.c_str());
        json readSettings;

        readFileStream >> readSettings;

        return JsonSettings(readSettings);
    }

    template <class T> operator T() const
    {
        return _jsonSettings.get<T>();
    }

protected:
    JsonSettings(json settings)
                :	_jsonSettings(settings)
    {
    }

private:
    json _jsonSettings;
};

void from_json(const json &j, Point &p)
{
    p.X = j.at("lon").get<double>();
    p.Y = j.at("lat").get<double>();
}

void from_json(const json& j, Area& area)
{
    area.SetDescription(j.at("description").get<std::string>());
    area.SetEPSG(j.at("epsg").get<std::string>());
    area.SetLeftTop(j.at("top_left").get<Point>());
    area.SetBottomRight(j.at("bottom_right").get<Point>());
}

void from_json(const json &j, StepSettings &stepSettings)
{
    stepSettings.SetName(j.at("name").get<std::string>());
    stepSettings.SetType(j.at("type").get<std::string>());
    stepSettings.SetLayerName(j.value("layer_name", ""));
    stepSettings.SetLayerUrl(j.value("layer_url", ""));
    stepSettings.SetLayerIndex(j.value("layer_index", -1));
    stepSettings.SetMaskingLayerName(j.value("masking_layer_name", ""));
    stepSettings.SetFileType(j.value("file_type", "GTiff"));
    stepSettings.SetOutputDirectory(j.value("output_directory", ""));
}

void from_json(const json& j, Settings& settings)
{
    settings.SetTileWidth(j.at("tile_width").get<int>());
    settings.SetTileHeight(j.at("tile_height").get<int>());
    settings.SetChosenArea(j.at("area").get<Area>());
    settings.SetStepSettingsCollection(j.at("steps").get<std::vector<StepSettings>>());
}

#endif // JSONSETTINGS_H
