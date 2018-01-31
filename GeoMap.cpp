#include "GeoMap.h"

GeoMap::GeoMap()
{
}

Area GeoMap::GetMapArea()
{
    return _mapArea;
}

void GeoMap::SetTitle(string title)
{
    _title = title;
}

void GeoMap::GetTilesForArea(const Area& area)
{
    //TODO convert area into rect, and get tiles for rect
    Rect fullAreaRect = RectForArea(area);
    for (auto& tileRect : GetTilesForRect(fullAreaRect))
    {
        Area tileArea = AreaForRect(tileRect);
        GetTileForRect(tileArea, tileRect);
    }
}


vector<Rect> GeoMap::GetTilesForRect(const Rect& rect)
{
    vector<Rect> tileRectangles;

    return tileRectangles;
}

GeoTile GeoMap::GetTileForRect(const Area& area, const Rect& rect)
{
//TODO
//    return GeoTile(area);
}
