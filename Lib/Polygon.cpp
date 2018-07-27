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

Polygon::operator OGRGeometry *() const
{
    OGRPolygon* ogrPolygon = new OGRPolygon();
    //Note: needed to work around GDAL memory management here a bit
    //the spatialReference needs to be passed as a pointer but is not copied
    //only the reference count is increased.
    OGRSpatialReference* reference = new OGRSpatialReference(_spatialReference.InnerReference());
    ogrPolygon->assignSpatialReference(reference);
    reference->Release();

    OGRGeometry* externalRing = _externalRing;
    ogrPolygon->addRing((OGRCurve*) externalRing);
    delete externalRing;

    for (auto& ring : _internalRings)
    {
        OGRGeometry* internalRing = ring;
        ogrPolygon->addRing((OGRCurve*) internalRing);
        delete internalRing;
    }
    return ogrPolygon;
}

Polygon& Polygon::operator=(const OGRGeometry *geometry)
{
    OGRPolygon* ogrPolygon = (OGRPolygon*)geometry;
    _externalRing = ogrPolygon->getExteriorRing();

    for (int i = 0; i < ogrPolygon->getNumInteriorRings(); i++)
    {
        Ring newInternalRing;
        newInternalRing = ogrPolygon->getInteriorRing(i);
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

Polygon Polygon::Transform(Geometry<Polygon>::TransformFunction transformFunction) const
{
    Ring mappedExternalRing = GetExternalRing().Transform(transformFunction);
    
    vector<Ring> mappedInternalRings;
    for (auto internalRing : GetInternalRings())
    {
        mappedInternalRings.push_back(internalRing.Transform(transformFunction));
    }
    return Polygon(mappedExternalRing, mappedInternalRings);
}
