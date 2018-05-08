#include "Polygon.h"

Polygon::Polygon()
{
}

Polygon::~Polygon()
{
}

Polygon::Ring::Ring()
{
}

Polygon::Ring::~Ring()
{
}

void Polygon::Ring::AddPoint(Point point)
{
    _points.push_back(point);
}

const Polygon::Ring& Polygon::ExternalRing() const
{
    return _externalRing;
}

const Polygon::Ring& Polygon::InternalRing() const
{
    return _internalRing;
}


