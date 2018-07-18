#ifndef STEPSETTINGS_H
#define STEPSETTINGS_H

#include <string>

class StepSettings
{
public:
    StepSettings();
    virtual ~StepSettings();

    std::string Name();
    void SetName(std::string name);

    std::string Type();
    void SetType(std::string type);

    std::string LayerName();
    void SetLayerName(std::string layerName);

    std::string LayerUrl();
    void SetLayerUrl(std::string layerUrl);

    int LayerIndex();
    void SetLayerIndex(int layerIndex);

    std::string MaskingLayerName() const;
    void SetMaskingLayerName(const std::string &maskingLayerName);

    std::string FileType() const;
    void SetFileType(const std::string &fileType);

    std::string OutputDirectory() const;
    void SetOutputDirectory(const std::string &outputDirectory);

private:
    std::string _name;
    std::string _type;
    std::string _layerName;
    std::string _layerUrl;
    int _layerIndex;

    std::string _maskingLayerName;
    std::string _fileType;
    std::string _outputDirectory;
};

#endif // STEPSETTINGS_H
