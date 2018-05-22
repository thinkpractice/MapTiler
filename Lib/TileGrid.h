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

        template <typename T>
        class TileGridIterator
        {
            public:
                using value_type = T;
                using difference_type = ptrdiff_t;
                using pointer = T*;
                using reference = T&;
                using iterator_category = input_iterator_tag;

                TileGridIterator(TileGrid* owner, bool start);
                virtual ~TileGridIterator();

                reference operator*() const { return _currentRect; };

                TileGridIterator& operator++();
                TileGridIterator operator++(int);

                bool operator==(const TileGridIterator& rhs);
                bool operator!=(const TileGridIterator& rhs);

            private:
                void NextField();

            private:
                TileGrid* _owner;
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
        Rect ClipTileDimensions(Rect tileRect);

    private:
        Rect _pixelDimensions;
        Area _area;
        int _tileWidth;
        int _tileHeight;

};

#endif
