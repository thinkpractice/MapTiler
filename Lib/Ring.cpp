#include "Ring.h"

Ring::Ring()
        :	Geometry(Geometry::RingType)
{
}

Ring::Ring(vector<Point> points)
        :	Geometry(Geometry::RingType),
            _points(points)
{
}

Ring::Ring(std::initializer_list<Point> list)
        :	Geometry(Geometry::RingType),
            _points(list)
{

}
Ring::Ring(std::vector<std::tuple<double, double>> list)
        :	Geometry(Geometry::RingType)
{
    for (auto p : list)
        _points.push_back(p);
}

Ring::Ring(OGRGeometry *ring)
        :	Geometry (Geometry::RingType)
{
    ParseGeometry(dynamic_cast<OGRLinearRing*>(ring));
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
    return operator=(dynamic_cast<const OGRLinearRing*>(ring));
}

void Ring::ParseGeometry(const OGRLinearRing* ring)
{
    Geometry::ParseGeometry(ring);
    _clockwise = ring->isClockwise() > 0;
    for (int i = 0; i < ring->getNumPoints(); i++)
    {
        OGRPoint point;
        ring->getPoint(i, &point);
        AddPoint(Point(point.getX(), point.getY()));
    }
}

Ring& Ring::operator=(const OGRLinearRing* ring)
{
    ParseGeometry(ring);
    return *this;
}

bool Ring::IsClockwise()
{
    return _clockwise;
}

shared_ptr<Geometry> Ring::Transform(Geometry::TransformFunction transformFunction) const
{
    vector<std::tuple<double, double>> originalPoints;
    for (auto p : _points)
        originalPoints.push_back(p);
    vector<std::tuple<double, double>> mappedPoints = transformFunction(originalPoints);
    return make_shared<Ring>(mappedPoints);
}

void Ring::AddPoint(const Point point)
{
    _points.push_back(point);
}

