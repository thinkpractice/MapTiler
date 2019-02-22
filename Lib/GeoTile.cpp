#include "GeoTile.h"
#include "Utils.h"
#include <iostream>
#include <cstring>

using namespace std;

GeoTile::GeoTile(const Rect& rect, const Area& area, int numberOfLayers)
            :   _area(area),
                _rect(rect),
                _numberOfLayers(numberOfLayers)
{
    _data = new unsigned char[NumberOfBytes()];
    std::memset(_data, 255, NumberOfBytes());
}

GeoTile::~GeoTile()
{
    delete[] _data;
}

size_t GeoTile::NumberOfBytes()
{
    return static_cast<size_t>(_rect.IntegerWidth() * _rect.IntegerHeight() * _numberOfLayers);
}

int GeoTile::NumberOfLayers()
{
    return _numberOfLayers;
}

Rect GeoTile::BoundingRect()
{
    return _rect;
}

Area GeoTile::BoundingArea()
{
    return _area;
}

unsigned char* GeoTile::Data()
{
    return _data;
}
