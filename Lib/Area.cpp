#include "Area.h"

Area::Area(const SpatialReference& projectionReference, const Point& leftTop, const Point& bottomRight)
        :   __spatialReference(projectionReference),
            _leftTop(leftTop),
            _bottomRight(bottomRight)
{
}

SpatialReference SpatialReference() const
{
    return _spatialReference;
}

Point Area::LeftTop() const
{
    return _leftTop;
}

Point Area::BottomRight() const
{
    return _bottomRight;
}

