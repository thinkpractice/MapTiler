#ifndef TILE_GRID_H
#define TILE_GRID_H

#include "GeoTile.h"

class TileGrid
{
    public:
        TileGrid();
        virtual ~TileGrid();

        int Width();
        int Height();

        void AddTile(GeoTile* geoTile);
        GeoTile* GetTile(int x, int y);


};

#endif
