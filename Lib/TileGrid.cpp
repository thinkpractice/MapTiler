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

template<typename T>
TileGrid::TileGridIterator<T>::TileGridIterator(TileGrid* owner, bool start)
                                    :   _owner(owner),
                                        _currentRow(-1),
                                        _currentColumn(-1)
{
    if (start)
    {
        _currentRow = 0;
        NextField();
    }
}

template<typename T>
TileGrid::TileGridIterator<T>::~TileGridIterator()
{
}

template<typename T>
TileGrid::TileGridIterator<T>& TileGrid::TileGridIterator<T>::operator++()
{
    NextField();
    return *this;
}

template<typename T>
TileGrid::TileGridIterator<T> TileGrid::TileGridIterator<T>::operator++(int)
{
    TileGrid::TileGridIterator<T> temp = *this;
    ++(*this);
    return temp;
}

template<typename T>
bool TileGrid::TileGridIterator<T>::operator==(const TileGridIterator& rhs)
{
    return (_currentRow = rhs._currentRow) && (_currentColumn == rhs._currentColumn);
}

template<typename T>
bool TileGrid::TileGridIterator<T>::operator!=(const TileGridIterator& rhs)
{
    return !(*this == rhs);
}

template<typename T>
void TileGrid::TileGridIterator<T>::NextField()
{
    _currentColumn++;
    if (_currentColumn >= _owner->TileWidth())
    {
        _currentRow++;
        _currentColumn = 0;
    }

    if (_currentRow >= _owner->TileHeight())
    {
        _currentRow = -1;
        _currentColumn = -1;
        return;
    }
    _currentRect = (*_owner)(_currentRow,_currentColumn);
}

Rect TileGrid::ClipTileDimensions(Rect tileRect)
{
    double width = tileRect.Right() < PixelDimensions().Right() ? tileRect.Right() : PixelDimensions().Right();
    tileRect.SetWidth(width);

    double height = tileRect.Bottom() < PixelDimensions().Bottom() ? tileRect.Bottom() : PixelDimensions().Bottom();
    tileRect.SetHeight(height);

    return tileRect;
}
