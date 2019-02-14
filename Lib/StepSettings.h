#ifndef STEPSETTINGS_H
#define STEPSETTINGS_H

#include <string>
#include <vector>

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

    std::string DriverName() const;
    void SetDriverName(const std::string &driverName);

    std::vector<std::string> DriverOptions() const;
    void SetDriverOptions(const std::vector<std::string>& driverOptions);

    int EpsgFormat() const;
    void SetEpsgFormat(const int epsgFormat);

    std::string FileExtension() const;
    void SetFileExtension(const std::string &fileExtension);

    std::string OutputDirectory() const;
    void SetOutputDirectory(const std::string &outputDirectory);

    int TileWidth() const;
    void SetTileWidth(int TileWidth);

    int TileHeight() const;
    void SetTileHeight(int TileHeight);

    int MapYear() const;
    void SetMapYear(int mapYear);

    std::string PersistenceUrl() const;
    void SetPersistenceUrl(const std::string &persistenceUrl);

    std::string PersistenceLayerName() const;
    void SetPersistenceLayerName(const std::string &PersistenceLayerName);

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
    std::string _driverName;
    std::vector<std::string> _driverOptions;
    int _epsgFormat;
    std::string _fileExtension;
    std::string _outputDirectory;

    std::string _persistenceUrl;
    std::string _persistenceLayerName;

    int _tileWidth;
    int _tileHeight;
    int _mapYear;
};

#endif // STEPSETTINGS_H
