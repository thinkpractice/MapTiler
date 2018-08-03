#include "Polygon.h"
#include <iostream>

using namespace std;

Polygon::Polygon()
            :	Geometry(Geometry::PolygonType)
{
}

Polygon::Polygon(Ring externalRing, vector<Ring> internalRings)
            :	Geometry(Geometry::PolygonType),
                _externalRing(externalRing),
                _internalRings(internalRings)
{
}

Polygon::Polygon(const OGRGeometry *geometry)
            :	Polygon()
{
    ParseGeometry(geometry);
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
    ogrPolygon->addRing(dynamic_cast<OGRCurve*>(externalRing));
    delete externalRing;

    for (auto& ring : _internalRings)
    {
        OGRGeometry* internalRing = ring;
        ogrPolygon->addRing(dynamic_cast<OGRCurve*>(internalRing));
        delete internalRing;
    }
    return ogrPolygon;
}

Polygon& Polygon::operator=(const OGRGeometry *geometry)
{
    ParseGeometry(geometry);
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

shared_ptr<Geometry> Polygon::Transform(Geometry::TransformFunction transformFunction) const
{
    shared_ptr<Ring> mappedExternalRing = dynamic_pointer_cast<Ring>(GetExternalRing().Transform(transformFunction));
    
    vector<Ring> mappedInternalRings;
    for (auto internalRing : GetInternalRings())
    {
        shared_ptr<Ring> mappedInternalRing = dynamic_pointer_cast<Ring>(internalRing.Transform(transformFunction));
        mappedInternalRings.push_back(*mappedInternalRing);
    }
    return make_shared<Polygon>(*mappedExternalRing, mappedInternalRings);
}

void Polygon::ParseGeometry(const OGRGeometry* geometry)
{
    Geometry::ParseGeometry(geometry);
    const OGRPolygon* ogrPolygon = dynamic_cast<const OGRPolygon*>(geometry);
    _externalRing = ogrPolygon->getExteriorRing();

    for (int i = 0; i < ogrPolygon->getNumInteriorRings(); i++)
    {
        Ring newInternalRing;
        newInternalRing = ogrPolygon->getInteriorRing(i);
        _internalRings.push_back(newInternalRing);
    }
}
