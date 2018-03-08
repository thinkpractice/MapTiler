#include "SpatialReference.h"

SpatialReference::SpatialReference(const char* geoCs)
{
    SetWellKnownGeogCS(geoCs);
}

SpatialReference::SpatialReference(const Point& coordinate)
                    :   _coordinate(coordinate)
{
}

SpatialReference::SpatialReference(const char* wktProjectionRef, const Point& coordinate)
                    :   _coordinate(coordinate)
{
    _spatialReference = OGRSpatialReference(wktProjectionRef);
}

void SpatialReference::SetWellKnownGeogCS(const char* geoCs)
{
    _spatialReference.SetWellKnownGeogCS(geoCs);
}

bool SpatialReference::IsSame(SpatialReference& reference)
{
    OGRSpatialReference otherReference = reference.InnerReference();
    return _spatialReference.IsSame(&otherReference);
}

void SpatialReference::SetCoordinate(const Point& coordinate)
{
    _coordinate = coordinate;
}

Point SpatialReference::Coordinate() const
{
    return _coordinate;
}

OGRSpatialReference SpatialReference::InnerReference()
{
    return _spatialReference;
}

