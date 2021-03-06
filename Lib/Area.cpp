#include "Area.h"

Area::Area()
        : Area(0.0, 0.0, 0.0, 0.0)
{
}

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

Area::Area(double minX, double minY, double maxX, double maxY, string epsgCode, string description)
        :   Area(SpatialReference::FromEPSG(epsgCode), Point(minX, minY), Point(maxX, maxY), description)
{
}

SpatialReference Area::ProjectionReference() const
{
    return _spatialReference;
}

void Area::SetProjectionReference(const SpatialReference &projectionReference)
{
    _spatialReference = projectionReference;
}

void Area::SetEPSG(string epsg)
{
    SetProjectionReference(SpatialReference::FromEPSG(epsg));
}

Point Area::LeftTop() const
{
    return _leftTop;
}

void Area::SetLeftTop(const Point &leftTop)
{
    _leftTop = leftTop;
}

Point Area::BottomRight() const
{
    return _bottomRight;
}

void Area::SetBottomRight(const Point &bottomRight)
{
    _bottomRight = bottomRight;
}

void Area::SetDescription(string description)
{
    _description = description;
}

string Area::Description() const
{
    return _description;
}
