#ifndef TILE_WRITER_H
#define TILE_WRITER_H

#include <string>
#include "GeoTile.h"

using namespace std;

class TileWriter
{
    public:
        TileWriter();
        void Save(GeoTile* tile, string filename);
};

#endif
