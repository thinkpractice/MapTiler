#ifndef GDAL_MAP_H
#define GDAL_MAP_H

#include <string>
#include <tuple>
#include "gdal_priv.h"
#include "GeoTile.h"
#include "GeoMap.h"

using namespace std;

class GDALMap : public GeoMap
{
    public:
        GDALMap(string filename);
        virtual ~GDALMap();

        int LayerCount();
        int WidthInPixels();
        int HeightInPixels();

        AffineTransform MapTransform();
        Area GetMapArea();

    protected:
        GeoTile* GetTileForRect(const Rect& rectangle, const Area& area);

        tuple<int, int> GetTileSize();
        Rect RectForArea(const Area& area);
        Area AreaForRect(const Rect& rect);

    private:
        GDALDataset* Dataset();
        GByte* GetDataForBand(int rasterIndex, int x, int y, int width, int height);
        SpatialReference RasterToProjectionCoord(double x, double y);

    private:
        string _filename;
        GDALDataset* _dataset;
};

#endif
