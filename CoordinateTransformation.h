#ifndef COORDINATE_TRANSFORMATION_H
#define COORDINATE_TRANSFORMATION_H

#include "SpatialReference.h"
#include "Area.h"

class CoordinateTransformation
{
    public:
        static SpatialReference MapCoordinate(SpatialReference other, string epsgCode);
        static Area MapArea(Area other, string epsgCode);
};

#endif
