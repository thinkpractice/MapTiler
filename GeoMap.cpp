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
    int tileWidth, tileHeight = 0;
    tie(tileWidth, tileHeight) = GetTileSize();
    for (int x = rect.X(); x <= rect.Width() && x < WidthInPixels() ; x += tileWidth)
    {
        for (int y = rect.Y(); y <= rect.Height() && y < HeightInPixels(); y += tileHeight)
        {
            tileRectangles.push_back(Rect(x, y, tileWidth, tileHeight));
        }
    }

    return tileRectangles;
}

