#include "GeoMap.h"

GeoMap::GeoMap(const SpatialReference& reference)
            :   _spatialReference(reference)
{
}

SpatialReference GetMapReference()
{
    return _spatialReference;
}

void SpatialReference::GetTilesForArea(const Area& area)
{
}
