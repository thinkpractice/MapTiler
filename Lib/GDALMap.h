#ifndef GDAL_MAP_H
#define GDAL_MAP_H

#include <string>
#include <tuple>
#include <memory>
#include "GeoTile.h"
#include "GeoMap.h"
#include "SpatialReference.h"
#include "Point.h"
#include "Layer.h"
#include <gdal/gdal_priv.h>

class GDALMap : public GeoMap
{
    public:
        GDALMap(std::string filename);
        GDALMap(std::string filename, GDALDataset* dataset);
        virtual ~GDALMap();

        GeoMap* Clone();
        int LayerCount();
        int RasterCount();
        int WidthInPixels();
        int HeightInPixels();

        SpatialReference ProjectionReference();
        void SetProjectionReference(const SpatialReference& reference);

        AffineTransform MapTransform();
        void SetMapTransform(const AffineTransform& affineTransform);

        Area GetMapArea();
        unique_ptr<GeoTile> GetTileForRect(const Rect& rectangle);
        void WriteTile(shared_ptr<GeoTile> tile);

        std::tuple<int, int> GetTileSize();
        Rect RectForArea(const Area& area);
        Area AreaForRect(const Rect& rect);

        std::shared_ptr<Layer> ExecuteQuery(std::string query);

    private:
        GDALDataset* Dataset();
        GByte* GetDataForBand(int rasterIndex, int x, int y, int width, int height);

        Point RasterToProjectionCoord(Point rasterCoord);
        Point ProjectionToRasterCoord(Point projectionCoord);
    private:
        GDALDataset* _dataset;
};

#endif
