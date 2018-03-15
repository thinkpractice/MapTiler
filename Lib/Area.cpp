#include "Area.h"

Area::Area(const SpatialReference& projectionReference, const Point& leftTop, const Point& bottomRight, string description)
        :   _spatialReference(projectionReference),
            _leftTop(leftTop),
            _bottomRight(bottomRight),
            _description(description)
{
}

Area::Area(const SpatialReference& projectionReference,
            const Point& leftTop,
            const Point& bottomRight)
        :   Area(projectionReference, leftTop, bottomRight, "")
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

void Area::SetDescription(string description)
{
    _description = description;
}

string Area::Description() const
{
    return _description;
}
