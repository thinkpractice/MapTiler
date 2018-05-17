#ifndef SPATIAL_REFERENCE_H
#define SPATIAL_REFERENCE_H

#include <string>
#include <ogr_spatialref.h>
#include "Point.h"

using namespace std;

class SpatialReference
{
    public:
        SpatialReference();
        SpatialReference(const char* wktProjectionRef);
        SpatialReference(const OGRSpatialReference* reference);

        void SetWellKnownGeogCS(const char* geoCs);
        bool IsSame(SpatialReference& reference);

        OGRSpatialReference InnerReference();
    
    public:
        static SpatialReference FromEPSG(string epsgCode);

    private:
        OGRSpatialReference _spatialReference;
        
};

#endif
