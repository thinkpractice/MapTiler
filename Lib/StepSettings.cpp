#include "StepSettings.h"

StepSettings::StepSettings()
{
}

StepSettings::~StepSettings()
{
}

std::string StepSettings::Name()
{
    return _name;
}

void StepSettings::SetName(std::string name)
{
    _name = name;
}

std::string StepSettings::Type()
{
    return _type;
}

void StepSettings::SetType(std::string type)
{
    _type = type;
}

std::string StepSettings::LayerName()
{
    return _layerName;
}

void StepSettings::SetLayerName(std::string layerName)
{
    _layerName = layerName;
}

std::string StepSettings::LayerUrl()
{
    return _layerUrl;
}

void StepSettings::SetLayerUrl(std::string url)
{
    _layerUrl = url;
}

int StepSettings::LayerIndex()
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
