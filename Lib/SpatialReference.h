#ifndef SPATIAL_REFERENCE_H
#define SPATIAL_REFERENCE_H

#include <string>
#include <gdal/ogr_spatialref.h>

using namespace std;

class SpatialReference
{
    public:
        SpatialReference();
        SpatialReference(const char* wktProjectionRef);
        SpatialReference(const OGRSpatialReference* reference);

        void SetWellKnownGeogCS(const char* geoCs);
        void ImportFromEPSG(int epsg);
        bool IsSame(SpatialReference& reference);

        OGRSpatialReference InnerReference() const;
        string ToWkt() const;

    public:
        static SpatialReference FromEPSG(int epsg);

    private:
        OGRSpatialReference _spatialReference;

};

#endif
