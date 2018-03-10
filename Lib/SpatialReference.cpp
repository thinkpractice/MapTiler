#include "SpatialReference.h"

SpatialReference::SpatialReference()
{
}

SpatialReference::SpatialReference(const char* wktProjectionRef)
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

OGRSpatialReference SpatialReference::InnerReference()
{
    return _spatialReference;
}

