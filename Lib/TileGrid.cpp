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

Rect TileGrid::PixelDimensions() const
{
    return _pixelDimensions;
}

Area TileGrid::GridArea()
{
    return _area;
}

int TileGrid::TileWidth() const
{
    return _tileWidth;
}

int TileGrid::TileHeight() const
{
    return _tileHeight;
}

int TileGrid::NumberOfTiles() const
{
    return WidthInTiles() * HeightInTiles();
}

int TileGrid::WidthInTiles() const
{
    return (int)ceil(_pixelDimensions.Width() / (1.0 * TileWidth()));
}

int TileGrid::HeightInTiles() const
{
    return (int)ceil(_pixelDimensions.Height() / (1.0 * TileHeight()));
}

Rect TileGrid::operator()(int row, int column) const
{
    //TODO: exception if row or column are out of bounds
    if (row > HeightInTiles() || column > WidthInTiles())
        cout << "row or column index out of bounds" << endl;
    double startX = PixelDimensions().Left() + (column * TileWidth());
    double startY = PixelDimensions().Top() + (row * TileHeight());

    Rect tileRect = Rect(startX, startY, TileWidth(), TileHeight());
    return ClipTileDimensions(tileRect);
}

TileGrid::iterator TileGrid::begin() const
{
    return {this, true};
}

TileGrid::iterator TileGrid::end() const
{
    return {this, false};
}

TileGrid::const_iterator TileGrid::cbegin() const
{
    return {this, true};
}

TileGrid::const_iterator TileGrid::cend() const
{
    return {this, false};
}

Rect TileGrid::ClipTileDimensions(Rect tileRect) const
{
    double width = tileRect.Right() < PixelDimensions().Right() ? tileRect.Width() : PixelDimensions().Width();
    tileRect.SetWidth(width);

    double height = tileRect.Bottom() < PixelDimensions().Bottom() ? tileRect.Height() : PixelDimensions().Height();
    tileRect.SetHeight(height);

    return tileRect;
}
