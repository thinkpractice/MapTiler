#ifndef GEOMAP_H
#define GEOMAP_H

#include "Area.h"
#include "Rect.h"
#include "GeoTile.h"
#include "SpatialReference.h"
#include "AffineTransform.h"
#include "Layer.h"
#include <tuple>
#include <vector>
#include <string>
#include <functional>
#include <memory>

using namespace std;

class GeoMap
{
    public:
        GeoMap(string filename);
        virtual ~GeoMap();

        string Filename();
        void SetFilename(string filename);

        string Title();
        void SetTitle(string title);

        Area ConvertToMapProjection(const Area& area);
        void GetTilesForArea(const Area& area, function<void(unique_ptr<GeoTile>, int, int)> callback);

        virtual GeoMap* Clone() = 0;
        virtual int LayerCount() = 0;
        virtual int RasterCount() = 0;
        virtual int WidthInPixels() = 0;
        virtual int HeightInPixels() = 0;

        virtual SpatialReference ProjectionReference() = 0;
        virtual void SetProjectionReference(const SpatialReference& reference) = 0;

        virtual AffineTransform MapTransform() = 0;
        virtual void SetMapTransform(const AffineTransform& affineTransform) = 0;
        virtual Area GetMapArea() = 0;

        vector<Rect> GetTileRectsForArea(const Area& area);
        vector<Rect> GetTilesForRect(const Rect& rect);
        virtual unique_ptr<GeoTile> GetTileForRect(const Rect& rect) = 0;
        virtual void WriteTile(shared_ptr<GeoTile> tile) = 0;

        virtual tuple<int, int> GetBlockSize() = 0;
        virtual Rect RectForArea(const Area& area) = 0;
        virtual Area AreaForRect(const Rect& rect) = 0;

        virtual shared_ptr<Layer> ExecuteQuery(string query) = 0;
    private:
        string _title;
        string _filename;

};

#endif
