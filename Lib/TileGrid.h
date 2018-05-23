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

        Rect PixelDimensions() const;
        Area GridArea();

        int TileWidth() const;
        int TileHeight() const;

        int NumberOfTiles() const;
        int WidthInTiles() const;
        int HeightInTiles() const;

        Rect operator()(int row, int column) const;

        template <typename T>
        class TileGridIterator
        {
            public:
                using value_type = T;
                using difference_type = ptrdiff_t;
                using pointer = T*;
                using reference = T&;
                using iterator_category = input_iterator_tag;

                TileGridIterator(const TileGrid* owner, bool start);
                virtual ~TileGridIterator();

                reference operator*() { return _currentRect; };

                TileGridIterator<T>& operator++();
                TileGridIterator<T> operator++(int);

                bool operator==(const TileGridIterator<T>& rhs);
                bool operator!=(const TileGridIterator<T>& rhs);

            private:
                void NextRect();

            private:
                const TileGrid* _owner;
                Rect _currentRect;
                int _currentRow;
                int _currentColumn;
        };

        using iterator = TileGridIterator<Rect>;
        using const_iterator = TileGridIterator<const Rect>;

        iterator begin() const;
        iterator end() const;
        const_iterator cbegin() const;
        const_iterator cend() const;

    private:
        Rect ClipTileDimensions(Rect tileRect) const;

    private:
        Rect _pixelDimensions;
        Area _area;
        int _tileWidth;
        int _tileHeight;

};

template<typename T>
TileGrid::TileGridIterator<T>::TileGridIterator(const TileGrid* owner, bool start)
                                    :   _owner(owner),
                                        _currentRow(-1),
                                        _currentColumn(-1)
{
    if (start)
    {
        _currentRow = 0;
        NextRect();
    }
}

template<typename T>
TileGrid::TileGridIterator<T>::~TileGridIterator()
{
}

template<typename T>
TileGrid::TileGridIterator<T>& TileGrid::TileGridIterator<T>::operator++()
{
    NextRect();
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
bool TileGrid::TileGridIterator<T>::operator==(const TileGridIterator<T>& rhs)
{
    return (_currentRow == rhs._currentRow) && (_currentColumn == rhs._currentColumn);
}

template<typename T>
bool TileGrid::TileGridIterator<T>::operator!=(const TileGridIterator<T>& rhs)
{
    return !(*this == rhs);
}

template<typename T>
void TileGrid::TileGridIterator<T>::NextRect()
{
    _currentColumn++;
    if (_currentColumn >= _owner->WidthInTiles())
    {
        _currentRow++;
        _currentColumn = 0;
    }

    if (_currentRow >= _owner->HeightInTiles())
    {
        _currentRow = -1;
        _currentColumn = -1;
        return;
    }
    _currentRect = (*_owner)(_currentRow,_currentColumn);
}

#endif
