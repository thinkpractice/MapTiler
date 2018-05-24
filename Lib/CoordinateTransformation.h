#ifndef COORDINATE_TRANSFORMATION_H
#define COORDINATE_TRANSFORMATION_H

#include "Point.h"
#include "SpatialReference.h"
#include "Area.h"
#include <vector>

using namespace std;

class CoordinateTransformation
{
    public:
        static Point MapCoordinate(SpatialReference sourceReference, 
                SpatialReference targetReference,
                Point sourceCoordinate);
        static vector<Point> MapCoordinates(SpatialReference sourceReference, SpatialReference targetReference, vector<Point> coordinates);
        static Area MapArea(Area other, string epsgCode);
        static Area MapArea(Area other, SpatialReference destinationReference);
};

#endif
