#ifndef GEOMAP_H
#define GEOMAP_H

#include "Area.h"
#include "Rect.h"
#include "GeoTile.h"
#include <vector>

using namespace std;

class GeoMap
{
    public:
        GeoMap(const Area& mapArea);
        
        Area GetMapArea();
        void GetTilesForArea(const Area& area);

    private:
        Rect RectForArea(const Area& area);
        Area AreaForRect(const Rect& rect);

        vector<Rect> GetTilesForRect(const Rect& rect);
        GeoTile GetTileForRect(const Area& area, const Rect& rect);

    private:
        Area _mapArea;

};

#endif
