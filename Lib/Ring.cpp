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

Ring& Ring::operator=(OGRGeometry* ring)
{
    return operator=((OGRLinearRing*) ring);
}

Ring& Ring::operator=(OGRLinearRing* ring)
{
    _clockwise = ring->isClockwise() > 0;
    for (int i = 0; i < ring->getNumPoints(); i++)
    {
        OGRPoint point;
        ring->getPoint(i, &point);
        AddPoint(Point(point.getX(), point.getY()));
    }
    return *this;
}

bool Ring::IsClockwise()
{
    return _clockwise;
}

Ring Ring::Transform(Geometry<Ring>::TransformFunction transformFunction) const
{
    vector<Point> mappedPoints = transformFunction(_points);
    return Ring(mappedPoints);
}

void Ring::AddPoint(const Point point)
{
    _points.push_back(point);
}

