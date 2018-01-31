#ifndef GDAL_MAP_H
#define GDAL_MAP_H

#include <string>
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
    private:
        GDALDataset* Dataset();
        GByte* GetDataForBand(int rasterIndex, int x, int y, int width, int height);

    private:
        string _filename;
        GDALDataset* _dataset;
};

#endif
