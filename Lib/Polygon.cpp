#include "Polygon.h"
#include <iostream>

using namespace std;

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

Polygon::Ring Polygon::Ring::Transform(TransformFunction transformFunction)
{
    vector<Point> mappedPoints = transformFunction(Points());
    for (size_t i = 0; i < mappedPoints.size(); i++)
        cout << "Point ("  << Points()[i].X << "," << Points()[i].Y << ") -> (" << mappedPoints[i].X << "," << mappedPoints[i].Y << ")";
    return Polygon::Ring(mappedPoints);
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

Polygon Polygon::Transform(TransformFunction transformFunction)
{
    Polygon::Ring mappedExternalRing = ExternalRing().Transform(transformFunction);
    
    vector<Polygon::Ring> mappedInternalRings;
    for (auto internalRing : InternalRings())
    {
        mappedInternalRings.push_back(internalRing.Transform(transformFunction));
    }
    return Polygon(mappedExternalRing, mappedInternalRings);
}
