#ifndef SPATIAL_REFERENCE_H
#define SPATIAL_REFERENCE_H

#include <string>

using namespace std;

class SpatialReference
{
    public:
        SpatialReference(string epsgCode);

        string EpsgCode();
        bool hasEpsgCode(string epsgCode);

    private:
        string _epsgCode;
};

#endif