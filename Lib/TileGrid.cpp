#include "TileGrid.h"
#include <cmath>
#include <iostream>

TileGrid::TileGrid(const Rect& pixelDimensions, const Area& area, int tileWidth, int tileHeight)
            :   _pixelDimensions(pixelDimensions),
                _area(area),
                _tileWidth(tileWidth),
                _tileHeight(tileHeight)
{
}

TileGrid::~TileGrid()
{
}

Rect TileGrid::PixelDimensions()
{
    return _pixelDimensions;
}

Area TileGrid::GridArea()
{
    return _area;
}

int TileGrid::TileWidth()
{
    return _tileWidth;
}

int TileGrid::TileHeight()
{
    return _tileHeight;
}

int TileGrid::WidthInTiles()
{
    return (int)ceil(_pixelDimensions.Width() / (1.0 * TileWidth()));
}

int TileGrid::HeightInTiles()
{
    return (int)ceil(_pixelDimensions.Height() / (1.0 * TileHeight()));
}

Rect TileGrid::operator()(int row, int column)
{
    //TODO: exception if row or column are out of bounds
    if (row > HeightInTiles() || column > WidthInTiles())
        cout << "row or column index out of bounds" << endl;
    int startX = PixelDimensions().Left() + column * TileWidth();
    int startY = PixelDimensions().Top() + row * TileHeight();
    
    Rect tileRect = Rect(startX, startY, TileWidth(), TileHeight());
    return ClipTileDimensions(tileRect);
}

Rect TileGrid::ClipTileDimensions(Rect tileRect)
{
    double width = tileRect.Right() < PixelDimensions().Right() ? tileRect.Right() : PixelDimensions().Right();
    tileRect.SetWidth(width);

    double height = tileRect.Bottom() < PixelDimensions().Bottom() ? tileRect.Bottom() : PixelDimensions().Bottom();
    tileRect.SetHeight(height);

    return tileRect;
}
