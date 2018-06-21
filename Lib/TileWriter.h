#ifndef TILE_WRITER_H
#define TILE_WRITER_H

#include <string>
#include <vector>
#include <memory>
#include "GeoTile.h"

using namespace std;

class GeoTileWriter
{
public:
    GeoTileWriter() {};
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

    bool HandlesFile(string filename);
    void Save(shared_ptr<GeoTile> tile, string filename);
};

#endif
