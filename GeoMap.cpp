#include "GeoMap.h"

GeoMap::GeoMap()
{
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
        GetTileForRect(tileRect, tileArea);
    }
}


vector<Rect> GeoMap::GetTilesForRect(const Rect& rect)
{
    vector<Rect> tileRectangles;

    return tileRectangles;
}

