#include "MultiPolygon.h"

MultiPolygon::MultiPolygon()
{
}


MultiPolygon::MultiPolygon(std::vector<Polygon> polygons)
                :   _polygons(polygons)
{
}

MultiPolygon::~MultiPolygon()
{
}

OGRGeometry *MultiPolygon::ToGdal()
{

}

void MultiPolygon::FromGdal(OGRGeometry *geometry)
{
    OGRMultiPolygon* multiPolygon = (OGRMultiPolygon*)geometry;
    for (int i = 0; i < multiPolygon->getNumGeometries(); i++)
    {
        OGRPolygon* ogrPolygon = (OGRPolygon*)multiPolygon->getGeometryRef(i);

        Polygon polygon;
        polygon.FromGdal(ogrPolygon);

        AddPolygon(polygon);
    }
}

void MultiPolygon::AddPolygon(Polygon polygon)
{
    _polygons.push_back(polygon);
}

MultiPolygon MultiPolygon::Transform(Ring::TransformFunction transformFunction)
{
    std::vector<Polygon> mappedPolygons;
    for (auto& polygon : _polygons)
    {
        mappedPolygons.push_back(polygon.Transform(transformFunction));
    }
    return MultiPolygon(mappedPolygons);
}
