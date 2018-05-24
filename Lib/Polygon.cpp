#include "Polygon.h"

Polygon::Polygon()
{
}

Polygon::Polygon(Ring externalRing, vector<Ring> internalRings)
            :   _externalRing(externalRing),
                _internalRings(internalRings)
{
}

Polygon::~Polygon()
{
}

Polygon::Ring::Ring()
{
}

Polygon::Ring::Ring(vector<Point> points)
                :   _points(points)
{
}

Polygon::Ring::~Ring()
{
}

void Polygon::Ring::AddPoint(const Point point)
{
    _points.push_back(point);
}

Polygon::Ring& Polygon::ExternalRing()
{
    return _externalRing;
}

vector<Polygon::Ring>& Polygon::InternalRings()
{
    return _internalRings;
}


