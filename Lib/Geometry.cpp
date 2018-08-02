#include "Geometry.h"

BaseGeometry::BaseGeometry()
{
}

BaseGeometry::BaseGeometry(const SpatialReference &reference)
                :	_spatialReference(reference)
{
}

BaseGeometry::~BaseGeometry()
{
}

SpatialReference BaseGeometry::GetSpatialReference() const
{
    return _spatialReference;
}

void BaseGeometry::SetSpatialReference(const SpatialReference& spatialReference)
{
    _spatialReference = spatialReference;
}

BaseGeometry::Type BaseGeometry::GetType() const
{
    return _type;
}

void BaseGeometry::SetType(BaseGeometry::Type type)
{
    _type = type;
}

BaseGeometry::Type ParseGeometryType(OGRGeometry *geometry)
{
    switch(wkbFlatten(geometry->getGeometryType()))
    {
        case wkbPoint:
            return BaseGeometry::Type::PointType;
        case wkbPolygon:
            return BaseGeometry::Type::PolygonType;
        case wkbMultiPolygon:
            return BaseGeometry::Type::MultiPolygonType;
        default:
            break;
    }
    return BaseGeometry::Type::Other;
}
