#ifndef SPATIAL_REFERENCE_H
#define SPATIAL_REFERENCE_H

#include <string>
#include <ogr_spatialref.h>
#include "Point.h"

using namespace std;

class SpatialReference
{
    public:
        SpatialReference(const char* geoCs);
        SpatialReference(const Point& coordinate);
        SpatialReference(const char* wktProjectionRef, const Point& coordinate);

        void SetWellKnownGeogCS(const char* geoCs);
        bool IsSame(SpatialReference& reference);
        void SetCoordinate(const Point& coordinate);
        Point Coordinate() const;

        OGRSpatialReference InnerReference();

    private:
        OGRSpatialReference _spatialReference;
        Point _coordinate;
        
};

#endif
