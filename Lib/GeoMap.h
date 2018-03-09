#ifndef GEOMAP_H
#define GEOMAP_H

#include "Area.h"
#include "Rect.h"
#include "GeoTile.h"
#include "SpatialReference.h"
#include "AffineTransform.h"
#include <tuple>
#include <vector>
#include <string>

using namespace std;

class GeoMap
{
    public:
        GeoMap(string filename);
        
        string Filename();
        void SetFilename(string filename);

        string Title();
        void SetTitle(string title);

        void GetTilesForArea(const Area& area);

        virtual int LayerCount() = 0;
        virtual int RasterCount() = 0;
        virtual int WidthInPixels() = 0;
        virtual int HeightInPixels() = 0;
        virtual SpatialReference ProjectionReference() = 0;
        virtual AffineTransform MapTransform() = 0;
        virtual Area GetMapArea() = 0;

    protected:
        virtual GeoTile* GetTileForRect(const Rect& rect) = 0;

        virtual tuple<int, int> GetTileSize() = 0;
        virtual Rect RectForArea(const Area& area) = 0;
        virtual Area AreaForRect(const Rect& rect) = 0;

        vector<Rect> GetTilesForRect(const Rect& rect);

    private:
        string _title;
        string _filename;

};

#endif
