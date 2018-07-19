#include "Settings.h"
#include "JsonSettings.h"

Settings::Settings()
{
}

Settings::~Settings()
{
}

int Settings::TileWidth() const
{
    return _tileWidth;
}

void Settings::SetTileWidth(int width)
{
    _tileWidth = width;
}

int Settings::TileHeight() const
{
    return _tileHeight;
}

void Settings::SetTileHeight(int height)
{
    _tileHeight = height;
}

Area Settings::ChosenArea() const
{
    return _chosenArea;
}

void Settings::SetChosenArea(const Area &area)
{
    _chosenArea = area;
}

std::vector<StepSettings> Settings::StepSettingsCollection() const
{
    return _stepSettings;
}

void Settings::SetStepSettingsCollection(const std::vector<StepSettings>& stepSettings)
{
    _stepSettings = stepSettings;
}

Settings Settings::Open(std::string filename)
{
    JsonSettings jsonSettings = JsonSettings::LoadFromFile(filename);
    Settings settings = jsonSettings;
    return settings;
}

std::string Settings::MainRasterName() const
{
    return _mainRasterName;
}

void Settings::SetMainRasterName(const std::string &mainRasterName)
{
    _mainRasterName = mainRasterName;
}

int Settings::MainRasterLayerIndex() const
{
    return _mainRasterLayerIndex;
}

void Settings::SetMainRasterLayerIndex(int mainRasterLayerIndex)
{
    _mainRasterLayerIndex = mainRasterLayerIndex;
}

std::string Settings::MainRasterUrl() const
{
    return _mainRasterUrl;
}

void Settings::SetMainRasterUrl(const std::string &mainRasterUrl)
{
    _mainRasterUrl = mainRasterUrl;
}
