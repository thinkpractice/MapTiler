#include "SpatialReference.h"
#include <cpl_conv.h>

SpatialReference::SpatialReference()
{
}

SpatialReference::SpatialReference(const char* wktProjectionRef)
{
    _spatialReference = OGRSpatialReference(wktProjectionRef);
}

SpatialReference::SpatialReference(const OGRSpatialReference* reference)
{
    if (reference)
        _spatialReference = *reference;
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

string SpatialReference::ToWkt() const
{
    char *wktString = nullptr;

    _spatialReference.exportToWkt(&wktString);
    string wkt(wktString);

    CPLFree(wktString);

    return wkt;
}

SpatialReference SpatialReference::FromEPSG(string epsgCode)
{
    SpatialReference reference;
    reference.SetWellKnownGeogCS(epsgCode.c_str());
    return reference;
}
