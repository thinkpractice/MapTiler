#ifndef SETTINGS_H
#define SETTINGS_H

#include <vector>
#include <string>
#include "StepSettings.h"
#include "Area.h"

class Settings
{
public:
    Settings();
    virtual ~Settings();

    int TileWidth() const;
    void SetTileWidth(int width);

    int TileHeight() const;
    void SetTileHeight(int height);

    Area ChosenArea() const;
    void SetChosenArea(const Area& area);

    std::vector<StepSettings> StepSettingsCollection() const;
    void SetStepSettingsCollection(const std::vector<StepSettings>& stepSettings);

    std::string MainRasterName() const;
    void SetMainRasterName(const std::string &mainRasterName);

    std::string MainRasterUrl() const;
    void SetMainRasterUrl(const std::string &mainRasterUrl);

    int MainRasterLayerIndex() const;
    void SetMainRasterLayerIndex(int mainRasterLayerIndex);

    std::string Address() const;
    void SetAddress(const std::string &Address);

    int AddressOption() const;
    void SetAddressOption(int AddressOption);

    std::string OutputDirectory() const;
    void SetOutputDirectory(const std::string &OutputDirectory);

public:
    static Settings Open(std::string filename);

private:
    std::string _mainRasterName;
    std::string _mainRasterUrl;
    int _mainRasterLayerIndex;
    int _tileWidth;
    int _tileHeight;

    std::string _address;
    int _addressOption;
    std::string _outputDirectory;

    Area _chosenArea;
    std::vector<StepSettings> _stepSettings;

};

#endif // SETTINGS_H
