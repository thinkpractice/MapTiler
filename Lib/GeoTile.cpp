#include "GeoTile.h"
#include "Utils.h"
#include <iostream>

using namespace std;

GeoTile::GeoTile(const Rect& rect, const Area& area, int numberOfLayers)
            :   _rect(rect),
                _area(area),
                _uniqueId(Utils::UUID()),
                _numberOfLayers(numberOfLayers)
{
    _data = new unsigned char[NumberOfBytes()];
}

GeoTile::~GeoTile()
{
    delete[] _data;
}

string GeoTile::UniqueId()
{
    return _uniqueId;
}

void GeoTile::SetUniqueId(string uniqueId)
{
    _uniqueId = uniqueId;
}

int GeoTile::NumberOfBytes()
{
    return int(_rect.Width()) * int(_rect.Height()) * _numberOfLayers;
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

void GeoTile::SetRasterData(unsigned char** rasterData)
{
    unsigned char* data = Data();
    for (int arrayIndex = 0, i = 0; arrayIndex < NumberOfBytes(); arrayIndex += NumberOfLayers(), i++)
    {
        for (int layerIndex = 0; layerIndex < NumberOfLayers(); layerIndex++)
        {
            data[arrayIndex+layerIndex] = rasterData[layerIndex][i];
        }
    }
}

unsigned char* GeoTile::GetRasterBand(int rasterIndex)
{
    int numberOfRasterBytes = NumberOfBytes() / NumberOfLayers();
    unsigned char* rasterData = new unsigned char[numberOfRasterBytes];
    if (rasterIndex > NumberOfLayers())
        return rasterData;

    unsigned char* data = Data();
    for (int arrayIndex = (rasterIndex-1), i = 0; arrayIndex < NumberOfBytes(); arrayIndex += NumberOfLayers(), i++)
    {
        rasterData[i] = data[arrayIndex];
    }

    return rasterData;
}
