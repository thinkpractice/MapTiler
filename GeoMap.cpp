#include "GeoMap.h"

GeoMap::GeoMap(const Area& mapArea)
            :   _mapArea(mapArea)
{
}

Area GetMapArea()
{
    return _mapArea;
}

void GeoMap::GetTilesForArea(const Area& area)
{
    //TODO convert area into rect, and get tiles for rect
    Rect fullAreaRect = GetPixelRangeForArea(area);
    for (auto& tileRect : GetTilesForRect(fullAreaRect))
    {
        GetTileForRect(tileRect);
    }
}

Rect GeoMap::GetPixelRangeForArea(const Area& area)
{
    return Rect(0, 0, 0, 0);
}

vector<Rect> GeoMap::GetTilesForRect(Rect rect)
{
    vector<Rect> tileRectangles;

    return tileRectangles;
}

GeoTile GeoMap::GetTileForRect(Rect rect)
{
    return GeoTile();
}
