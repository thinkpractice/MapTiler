#ifndef GEOMAP_H
#define GEOMAP_H

#include "Area.h"
#include "Rect.h"
#include <vector>

using namespace std;

class GeoMap
{
    public:
        GeoMap(const Area& mapArea);
        
        Area GetMapArea();
        void GetTilesForArea(const Area& area);

    private:
        Rect GetPixelRangeForArea(const Area& area);
        vector<Rect> GetTilesForRect(Rect rect);
        GeoTile GetTileForRect(Rect rect);

    private:
        Area _mapArea;

};

#endif
