#ifndef TILE_WRITER_H
#define TILE_WRITER_H

#include <string>
#include <memory>
#include "GeoTile.h"

using namespace std;

class TileWriter
{
    public:
        TileWriter();
        void Save(shared_ptr<GeoTile> tile, string filename);
};

#endif
