#ifndef GEOMAP_H
#define GEOMAP_H

#include "SpatialReference.h"

class GeoMap
{
    public:
        GeoMap(const SpatialReference &reference);
        
        SpatialReference GetMapReference();
        void GetTilesForArea(const Area& area);

    private:
        SpatialReference _spatialReference;

};

#endif
