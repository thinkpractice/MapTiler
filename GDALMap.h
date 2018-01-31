#ifndef GDAL_MAP_H
#define GDAL_MAP_H

#include <string>
#include <tuple>
#include "gdal_priv.h"

using namespace std;

class GDALMap
{
    public:
        GDALMap(string filename);
        virtual ~GDALMap();

        int LayerCount();
        int WidthInPixels();
        int HeightInPixels();

        GeoTile* GetTile(const Rect& rectangle, const Area& area);

    private:
        GDALDataset* Dataset();
        tuple<int, int> GDALMap::GetTileSize();
        GByte* GetDataForBand(int rasterIndex, int x, int y, int width, int height);

    private:
        string _filename;
        GDALDataset* _dataset;
};

#endif
