#include "GeoMap.h"
#include <iostream>
#include "CoordinateTransformation.h"

using namespace std;

GeoMap::GeoMap(string filename)
            :   _filename(filename),
                _title(filename)
{
}

GeoMap::~GeoMap()
{
}

string GeoMap::Filename()
{
    return _filename;
}

void GeoMap::SetFilename(string filename)
{
    _filename = filename;
}

string GeoMap::Title()
{
    return _title;
}

void GeoMap::SetTitle(string title)
{
    _title = title;
}

Area GeoMap::ConvertToMapProjection(const Area& area)
{
    return CoordinateTransformation::MapArea(area, ProjectionReference());
}

vector<GeoTile*> GeoMap::GetTilesForArea(const Area& area)
{
    Area projectedArea = ConvertToMapProjection(area);

    vector <GeoTile*> tiles;
    Rect fullAreaRect = RectForArea(projectedArea);
    cout << "fullAreaRect= " << fullAreaRect.Left() << "," << fullAreaRect.Top() << "," << fullAreaRect.Width() << "," << fullAreaRect.Height() << endl;

    for (auto& tileRect : GetTilesForRect(fullAreaRect))
    {
        //TODO store reference to geoTile somewhere and return them
        GeoTile* geoTile = GetTileForRect(tileRect);
        tiles.push_back(geoTile);
    }
    return tiles;
}

vector<Rect> GeoMap::GetTilesForRect(const Rect& rect)
{
    vector<Rect> tileRectangles;
    int tileWidth, tileHeight = 0;
    tie(tileWidth, tileHeight) = GetTileSize();
    for (int x = rect.Left(); x <= rect.Right() && x < WidthInPixels() ; x += tileWidth)
    {
        for (int y = rect.Top(); y <= rect.Bottom() && y < HeightInPixels(); y += tileHeight)
        {
            Rect tileRect = Rect(x, y, tileWidth, tileHeight);

            //clip tile width if tile goes over right border
            if (tileRect.Right() > WidthInPixels())
                tileRect.SetWidth(WidthInPixels()-x);

            //clip tile height if tile goes over bottom border
            if (tileRect.Height() > HeightInPixels())
                tileRect.SetHeight(HeightInPixels()-y);

            tileRectangles.push_back(tileRect);
        }
    }

    cout << "numberOfTileRectangles=" << tileRectangles.size() << endl;
    return tileRectangles;
}

