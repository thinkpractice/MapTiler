#ifndef SETTINGS_H
#define SETTINGS_H

#include <vector>
#include <string>
#include "Area.h"
#include "StepSettings.h"

class Settings
{
public:
    Settings();
    virtual ~Settings();

    int TileWidth();
    void SetTileWidth(int width);

    int TileHeight();
    void SetTileHeight(int height);

    Area ChosenArea();
    void SetChosenArea(const Area& area);

    std::vector<StepSettings> StepSettingsCollection();
    void SetStepSettingsCollection(std::vector<StepSettings> stepSettings);

public:
    static Settings Open(std::string filename);

private:
    int _tileWidth;
    int _tileHeight;
    Area _chosenArea;
    std::vector<StepSettings> _stepSettings;

};

#endif // SETTINGS_H
