#include "GeoTile.h"

GeoTile::GeoTile(const Rect& rect, const Area& area, int numberOfLayers)
            :   _rect(rect),
                _area(area),
                _numberOfLayers(numberOfLayers)
{
    int numberOfBytes = _rect.Width * _rect.Height * _numberOfLayers;
    _data = new unsigned char[numberOfBytes];
}

GeoTile::~GeoTile()
{
    delete[] _data;
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

void GeoTile::SetRasterData(unsigned char* rasterData)
{
    unsigned char* data = geoTile->Data();
    for (int arrayIndex = 0, int i = 0; arrayIndex < arrayLength; arrayIndex += LayerCount(), i++)
    {
        for (int layerIndex = 0; layerIndex < NumberOfLayers(); layerIndex++)
        {
            data[arrayIndex+layerIndex] = rasterData[layerIndex][i];
        }
    }
}
