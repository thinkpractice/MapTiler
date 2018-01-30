#ifndef GEOMAP_H
#define GEOMAP_H

#include "SpatialReference.h"

class GeoMap
{
    public:
        GeoMap(SpatialReference &reference);
        
        SpatialReference GetMapReference();

    private:
        SpatialReference _spatialReference;

};

#endif
