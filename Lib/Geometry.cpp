#include "Geometry.h"

Geometry::Geometry(const Geometry::Type& type)
                :	_type(type)
{
}

Geometry::Geometry(const Geometry::Type& geometryType, const SpatialReference &reference)
                :   _spatialReference(reference),
                    _type(geometryType)
{
}

Geometry::~Geometry()
{
}

SpatialReference Geometry::GetSpatialReference() const
{
    return _spatialReference;
}

void Geometry::SetSpatialReference(const SpatialReference& spatialReference)
{
    _spatialReference = spatialReference;
}

Geometry::Type Geometry::GetType() const
{
    return _type;
}

bool Geometry::IsPoint()
{
    return GetType() == PointType;
}

bool Geometry::IsRing()
{
    return GetType() == RingType;
}

bool Geometry::IsPolygon()
{
    return GetType() == PolygonType;
}

bool Geometry::IsMultiPolygon()
{
    return GetType() == MultiPolygonType;
}

bool Geometry::IsOther()
{
    return GetType() == Other;
}

void Geometry::SetType(Geometry::Type type)
{
    _type = type;
}

Geometry::Type Geometry::ParseGeometryType(OGRGeometry *geometry)
{
    switch(wkbFlatten(geometry->getGeometryType()))
    {
        case wkbPoint:
            return Geometry::Type::PointType;
        case wkbPolygon:
            return Geometry::Type::PolygonType;
        case wkbMultiPolygon:
            return Geometry::Type::MultiPolygonType;
        default:
            break;
    }
    return Geometry::Type::Other;
}

GeometryFactory::GeometryFactory()
{
}

GeometryFactory::~GeometryFactory()
{
}
