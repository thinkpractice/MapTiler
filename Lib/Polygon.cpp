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

Polygon& Polygon::operator=(const OGRGeometry *geometry)
{
    OGRPolygon* ogrPolygon = (OGRPolygon*)geometry;
    OGRLinearRing* ring = ogrPolygon->getExteriorRing();
    _externalRing = GetRingFromGdal(ring);

    for (int i = 0; i < ogrPolygon->getNumInteriorRings(); i++)
    {
        OGRLinearRing* internalRing = ogrPolygon->getInteriorRing(i);
        Ring newInternalRing = GetRingFromGdal(internalRing);
        _internalRings.push_back(newInternalRing);
    }
    return *this;
}

Ring& Polygon::ExternalRing()
{
    return _externalRing;
}

vector<Ring>& Polygon::InternalRings()
{
    return _internalRings;
}

Ring Polygon::GetExternalRing() const
{
    return _externalRing;
}

vector<Ring> Polygon::GetInternalRings() const
{
    return _internalRings;
}

Polygon Polygon::Transform(Ring::TransformFunction transformFunction)
{
    Ring mappedExternalRing = ExternalRing().Transform(transformFunction);
    
    vector<Ring> mappedInternalRings;
    for (auto internalRing : InternalRings())
    {
        mappedInternalRings.push_back(internalRing.Transform(transformFunction));
    }
    return Polygon(mappedExternalRing, mappedInternalRings);
}
