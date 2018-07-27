#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "ogrsf_frmts.h"
#include <memory>
#include "SpatialReference.h"
#include "Ring.h"

class Geometry
{
public:
    Geometry();
    Geometry(const SpatialReference& spatialReference);
    virtual ~Geometry();

    virtual operator OGRGeometry*() const = 0;

    Ring GetRingFromGdal(OGRLinearRing* ring);

    SpatialReference GetSpatialReference();
    void SetSpatialReference(const SpatialReference& spatialReference);

protected:
    SpatialReference _spatialReference;
};

#endif // GEOMETRY_H
