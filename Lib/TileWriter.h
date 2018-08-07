#ifndef TILE_WRITER_H
#define TILE_WRITER_H

#include <string>
#include <vector>
#include <memory>
#include <gdal/cpl_string.h>
#include "GeoTile.h"
#include "GDALMap.h"
#include "SpatialReference.h"

using namespace std;

class GeoTileWriter
{
public:
    GeoTileWriter();
    virtual ~GeoTileWriter();
    virtual bool HandlesFile(string filename) = 0;
    virtual void Save(shared_ptr<GeoTile> tile, string filename) = 0;

    virtual std::string DriverName() const = 0;
    virtual std::string FileExtension() const = 0;
};

class TileWriter
{
    public:
        TileWriter(shared_ptr<GeoTileWriter> writer);
        void Save(shared_ptr<GeoTile> tile, string filename);

        std::string FileExtension() const;

    private:
        shared_ptr<GeoTileWriter> _writer;
};

class GdalWriter : public GeoTileWriter
{
public:
    GdalWriter(std::string epsgFormat = "EPSG:4326", std::string driverName = "GTiff", std::string fileExtension = "tiff");
    GdalWriter(const SpatialReference& targetProjection, std::string driverName = "GTiff", std::string fileExtension = "tiff");
    virtual ~GdalWriter();

    bool HandlesFile(string filename);
    void Save(shared_ptr<GeoTile> tile, string filename);
    std::string DriverName() const;
    std::string FileExtension() const;

private:
    shared_ptr<GeoMap> MapFor(shared_ptr<GeoTile> tile, string filename);
    GDALDriver* DriverFor(string fileFormat);
    bool SupportsCreate(GDALDriver* driver);

private:
    SpatialReference _targetProjection;
    std::string _driverName;
    std::string _fileExtension;
};

#endif
