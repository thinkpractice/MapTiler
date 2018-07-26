#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "ogrsf_frmts.h"
#include <memory>
#include "Point.h"
#include "Ring.h"

class Geometry
{
public:

    Geometry();
    virtual ~Geometry();

    virtual OGRGeometry* ToGdal() = 0;
    virtual void FromGdal(OGRGeometry* geometry) = 0;

    Ring GetRingFromGdal(OGRLinearRing* ring);
};

#endif // GEOMETRY_H
