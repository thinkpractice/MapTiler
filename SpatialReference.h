#ifndef SPATIAL_REFERENCE_H
#define SPATIAL_REFERENCE_H

#include <string>

class SpatialReference
{
    public:
        SpatialReference(string epsgCode);

        string EpsgCode();

    private:
        string _epsgCode;
};

#endif
