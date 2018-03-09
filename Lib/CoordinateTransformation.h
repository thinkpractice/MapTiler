#ifndef COORDINATE_TRANSFORMATION_H
#define COORDINATE_TRANSFORMATION_H

#include "Point.h"
#include "SpatialReference.h"
#include "Area.h"

class CoordinateTransformation
{
    public:
        static Point MapCoordinate(SpatialReference sourceReference, 
                SpatialReference targetReference,
                Point sourceCoordinate);
        static Area MapArea(Area other, string epsgCode);
};

#endif
