#include "Area.h"

Area::Area(const SpatialReference& projectionReference, const Point& leftTop, const Point& bottomRight)
        :   _spatialReference(projectionReference),
            _leftTop(leftTop),
            _bottomRight(bottomRight)
{
}

SpatialReference Area::ProjectionReference() const
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

