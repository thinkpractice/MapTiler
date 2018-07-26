#include "Ring.h"

Ring::Ring()
{
}

Ring::Ring(vector<Point> points)
                :   _points(points)
{
}

Ring::Ring(std::initializer_list<Point> list)
                :	_points(list)
{

}

Ring::~Ring()
{
}

Ring::operator OGRGeometry *() const
{
    OGRLinearRing* ogrRing = new OGRLinearRing();
    for (auto& point : _points)
       ogrRing->addPoint(point.X, point.Y);
    return ogrRing;
}

Ring Ring::Transform(TransformFunction transformFunction)
{
    vector<Point> mappedPoints = transformFunction(Points());
    return Ring(mappedPoints);
}

void Ring::AddPoint(const Point point)
{
    _points.push_back(point);
}

