#include "Settings.h"
#include "JsonSettings.h"

Settings::Settings()
{
}

Settings::~Settings()
{
}

int Settings::TileWidth()
{
    return _tileWidth;
}

void Settings::SetTileWidth(int width)
{
    _tileWidth = width;
}

int Settings::TileHeight()
{
    return _tileHeight;
}

void Settings::SetTileHeight(int height)
{
    _tileHeight = height;
}

Area Settings::ChosenArea()
{
    return _chosenArea;
}

void Settings::SetChosenArea(const Area &area)
{
    _chosenArea = area;
}

vector<StepSettings> Settings::StepSettingsCollection()
{
    return _stepSettings;
}

void Settings::SetStepSettingsCollection(std::vector<StepSettings> stepSettings)
{
    _stepSettings = stepSettings;
}

Settings Settings::Open(std::string filename)
{
    JsonSettings jsonSettings = JsonSettings::LoadFromFile(filename);
    Settings settings = jsonSettings;
    return settings;
}
