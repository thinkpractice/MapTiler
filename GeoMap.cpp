#include "GeoMap.h"

GeoMap::GeoMap(const Area& mapArea)
            :   _mapArea(mapArea)
{
}

Area GeoMap::GetMapArea()
{
    return _mapArea;
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

Rect GeoMap::RectForArea(const Area& area)
{
    return Rect(0, 0, 0, 0);
}

Area GeoMap::AreaForRect(const Rect& rect)
{
}

vector<Rect> GeoMap::GetTilesForRect(const Rect& rect)
{
    vector<Rect> tileRectangles;

    return tileRectangles;
}

GeoTile GeoMap::GetTileForRect(const Area& area, const Rect& rect)
{
    return GeoTile(area);
}
