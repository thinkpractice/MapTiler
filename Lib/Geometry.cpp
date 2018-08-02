#include "Geometry.h"

Geometry::Geometry(const Geometry::Type& type)
                :	_type(type)
{
}

Geometry::Geometry(const Geometry::Type& type, const SpatialReference &reference)
                :	_type(type),
                    _spatialReference(reference)
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

void Geometry::SetType(Geometry::Type type)
{
    _type = type;
}

Geometry::Type ParseGeometryType(OGRGeometry *geometry)
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
