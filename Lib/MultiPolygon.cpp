#include "MultiPolygon.h"

MultiPolygon::MultiPolygon()
{
}


MultiPolygon::MultiPolygon(vector<Polygon> polygons)
                :   _polygons(polygons)
{
}

MultiPolygon::~MultiPolygon()
{
}

void MultiPolygon::AddPolygon(Polygon polygon)
{
    _polygons.push_back(polygon);
}

MultiPolygon MultiPolygon::Transform(Polygon::TransformFunction transformFunction)
{
    vector<Polygon> mappedPolygons;
    for (auto& polygon : _polygons)
    {
        mappedPolygons.push_back(polygon.Transform(transformFunction));
    }
    return MultiPolygon(mappedPolygons);
}
