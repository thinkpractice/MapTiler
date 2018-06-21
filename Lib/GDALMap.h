#ifndef GDAL_MAP_H
#define GDAL_MAP_H

#include <string>
#include <tuple>
#include "gdal_priv.h"
#include "GeoTile.h"
#include "GeoMap.h"
#include "SpatialReference.h"
#include "Point.h"

using namespace std;

class GDALMap : public GeoMap
{
    public:
        GDALMap(string filename);
        GDALMap(string filename, GDALDataset* dataset);
        virtual ~GDALMap();

        GeoMap* Clone();
        int LayerCount();
        int RasterCount();
        int WidthInPixels();
        int HeightInPixels();

        SpatialReference ProjectionReference();
        AffineTransform MapTransform();
        Area GetMapArea();
        GeoTile* GetTileForRect(const Rect& rectangle);
        void WriteTile(GeoTile* tile);

        tuple<int, int> GetTileSize();
        Rect RectForArea(const Area& area);
        Area AreaForRect(const Rect& rect);

    private:
        GDALDataset* Dataset();
        GByte* GetDataForBand(int rasterIndex, int x, int y, int width, int height);

        Point RasterToProjectionCoord(Point rasterCoord);
        Point ProjectionToRasterCoord(Point projectionCoord);
    private:
        GDALDataset* _dataset;
};

#endif
