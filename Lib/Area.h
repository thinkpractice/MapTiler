#ifndef AREA_H
#define AREA_H

#include "SpatialReference.h"
#include "Point.h"

class Area
{
    public:
        Area(const SpatialReference& projectionReference, const Point& leftTop, const Point &bottomRight);

        SpatialReference ProjectionReference() const;
        Point LeftTop() const;
        Point BottomRight() const;

    private:
        SpatialReference _spatialReference;
        Point _leftTop;
        Point _bottomRight;

};

#endif
