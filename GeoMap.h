#ifndef GEOMAP_H
#define GEOMAP_H

#include "Area.h"
#include "Rect.h"
#include "GeoTile.h"
#include <vector>
#include <string>

using namespace std;

class GeoMap
{
    public:
        GeoMap();
        
        void SetTitle(string title);
        void GetTilesForArea(const Area& area);

        virtual Area GetMapArea() = 0;

    protected:
        virtual GeoTile* GetTileForRect(const Rect& rect, const Area& area) = 0;

        virtual Rect RectForArea(const Area& area) = 0;
        virtual Area AreaForRect(const Rect& rect) = 0;

        vector<Rect> GetTilesForRect(const Rect& rect);

    private:
        string _title;

};

#endif
