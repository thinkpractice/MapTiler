#include "StepSettings.h"

int StepSettings::TileWidthNotSet = 0;
int StepSettings::TileHeightNotSet = 0;

StepSettings::StepSettings()
{
}

StepSettings::~StepSettings()
{
}

std::string StepSettings::Name() const
{
    return _name;
}

void StepSettings::SetName(std::string name)
{
    _name = name;
}

std::string StepSettings::Type() const
{
    return _type;
}

void StepSettings::SetType(std::string type)
{
    _type = type;
}

std::string StepSettings::LayerName() const
{
    return _layerName;
}

void StepSettings::SetLayerName(std::string layerName)
{
    _layerName = layerName;
}

std::string StepSettings::LayerUrl() const
{
    return _layerUrl;
}

void StepSettings::SetLayerUrl(std::string url)
{
    _layerUrl = url;
}

int StepSettings::LayerIndex() const
{
    return _layerIndex;
}

void StepSettings::SetLayerIndex(int layerIndex)
{
    _layerIndex = layerIndex;
}

std::string StepSettings::MaskingLayerName() const
{
    return _maskingLayerName;
}

void StepSettings::SetMaskingLayerName(const std::string &maskingLayerName)
{
    _maskingLayerName = maskingLayerName;
}

std::string StepSettings::FileType() const
{
    return _fileType;
}

void StepSettings::SetFileType(const std::string &fileType)
{
    _fileType = fileType;
}

std::string StepSettings::OutputDirectory() const
{
    return _outputDirectory;
}

void StepSettings::SetOutputDirectory(const std::string &outputDirectory)
{
    _outputDirectory = outputDirectory;
}

int StepSettings::TileWidth() const
{
    return _tileWidth;
}

void StepSettings::SetTileWidth(int tileWidth)
{
    _tileWidth = tileWidth;
}

int StepSettings::TileHeight() const
{
    return _tileHeight;
}

void StepSettings::SetTileHeight(int tileHeight)
{
    _tileHeight = tileHeight;
}

int StepSettings::MapYear() const
{
    return _mapYear;
}

void StepSettings::SetMapYear(int mapYear)
{
    _mapYear = mapYear;
}

std::string StepSettings::PersistenceUrl() const
{
    return _persistenceUrl;
}

void StepSettings::SetPersistenceUrl(const std::string &persistenceUrl)
{
    _persistenceUrl = persistenceUrl;
}

std::string StepSettings::PersistenceLayerName() const
{
    return _persistenceLayerName;
}

void StepSettings::SetPersistenceLayerName(const std::string &persistenceLayerName)
{
    _persistenceLayerName = persistenceLayerName;
}
