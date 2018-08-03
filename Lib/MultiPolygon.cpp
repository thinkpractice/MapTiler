#include "MultiPolygon.h"

MultiPolygon::MultiPolygon()
                :	Geometry(Geometry::MultiPolygonType)
{
}

MultiPolygon::MultiPolygon(const OGRGeometry *geometry)
                :	Geometry(Geometry::MultiPolygonType)
{
    ParseGeometry(geometry);
}

MultiPolygon::MultiPolygon(std::vector<Polygon> polygons)
                :	Geometry(Geometry::MultiPolygonType),
                   _polygons(polygons)
{
}

MultiPolygon::~MultiPolygon()
{
}

MultiPolygon::operator OGRGeometry *() const
{
    OGRMultiPolygon *ogrMultiPolygon = new OGRMultiPolygon();
    for (auto& polygon : _polygons)
    {
        OGRGeometry* ogrPolygon = polygon;
        ogrMultiPolygon->addGeometry(ogrPolygon);
        delete ogrPolygon;
    }
    return ogrMultiPolygon;
}

MultiPolygon& MultiPolygon::operator=(const OGRGeometry *geometry)
{
    ParseGeometry(geometry);
    return *this;
}

void MultiPolygon::AddPolygon(Polygon polygon)
{
    _polygons.push_back(polygon);
}

shared_ptr<Geometry> MultiPolygon::Transform(Geometry::TransformFunction transformFunction) const
{
    std::vector<Polygon> mappedPolygons;
    for (auto& polygon : _polygons)
    {
        shared_ptr<Polygon> mappedPolygon = dynamic_pointer_cast<Polygon>(polygon.Transform(transformFunction));
        mappedPolygons.push_back(*mappedPolygon);
    }
    return make_shared<MultiPolygon>(mappedPolygons);
}

void MultiPolygon::ParseGeometry(const OGRGeometry* geometry)
{
    Geometry::ParseGeometry(geometry);
    const OGRMultiPolygon* multiPolygon = dynamic_cast<const OGRMultiPolygon*>(geometry);
    for (int i = 0; i < multiPolygon->getNumGeometries(); i++)
    {
        Polygon polygon;
        polygon = multiPolygon->getGeometryRef(i);
        AddPolygon(polygon);
    }
}
