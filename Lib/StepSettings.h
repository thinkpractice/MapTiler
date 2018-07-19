#ifndef STEPSETTINGS_H
#define STEPSETTINGS_H

#include <string>

class StepSettings
{
public:
    StepSettings();
    virtual ~StepSettings();

    std::string Name() const;
    void SetName(std::string name);

    std::string Type() const;
    void SetType(std::string type);

    std::string LayerName() const;
    void SetLayerName(std::string layerName);

    std::string LayerUrl() const;
    void SetLayerUrl(std::string layerUrl);

    int LayerIndex() const;
    void SetLayerIndex(int layerIndex);

    std::string MaskingLayerName() const;
    void SetMaskingLayerName(const std::string &maskingLayerName);

    std::string FileType() const;
    void SetFileType(const std::string &fileType);

    std::string OutputDirectory() const;
    void SetOutputDirectory(const std::string &outputDirectory);

    int TileWidth() const;
    void SetTileWidth(int TileWidth);

    int TileHeight() const;
    void SetTileHeight(int TileHeight);

public:
    static int TileWidthNotSet;
    static int TileHeightNotSet;

private:
    std::string _name;
    std::string _type;
    std::string _layerName;
    std::string _layerUrl;
    int _layerIndex;

    std::string _maskingLayerName;
    std::string _fileType;
    std::string _outputDirectory;

    int _tileWidth;
    int _tileHeight;
};

#endif // STEPSETTINGS_H