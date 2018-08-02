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
};

class TileWriter
{
    public:
        TileWriter(shared_ptr<GeoTileWriter> writer);
        void Save(shared_ptr<GeoTile> tile, string filename);

    private:
        shared_ptr<GeoTileWriter> _writer;
};

class PngWriter : public GeoTileWriter
{
public:
    PngWriter();
    virtual ~PngWriter();

    bool HandlesFile(string filename);
    void Save(shared_ptr<GeoTile> tile, string filename);
};

class GdalWriter : public GeoTileWriter
{
public:
    GdalWriter();
    GdalWriter(const SpatialReference& targetProjection);
    virtual ~GdalWriter();

    bool HandlesFile(string filename);
    void Save(shared_ptr<GeoTile> tile, string filename);

private:
    shared_ptr<GeoMap> MapFor(shared_ptr<GeoTile> tile, string filename);
    GDALDriver* DriverFor(string fileFormat);
    bool SupportsCreate(GDALDriver* driver);

private:
    SpatialReference _targetProjection;
};

#endif
