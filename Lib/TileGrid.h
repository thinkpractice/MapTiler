#ifndef TILE_GRID_H
#define TILE_GRID_H

#include "GeoTile.h"
#include "Rect.h"
#include "Area.h"

class TileGrid
{
    public:
        TileGrid(const Rect& pixelDimensions, const Area& area, int tileWidth, int tileHeight);
        virtual ~TileGrid();

        Rect PixelDimensions();
        Area GridArea();

        int TileWidth();
        int TileHeight();

        int WidthInTiles();
        int HeightInTiles();

        Rect operator()(int row, int column);

    private:
        Rect ClipTileDimensions(Rect tileRect);

    private:
        Rect _pixelDimensions;
        Area _area;
        int _tileWidth;
        int _tileHeight;

};

#endif
