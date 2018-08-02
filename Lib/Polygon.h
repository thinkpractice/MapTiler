#ifndef POLYGON_H
#define POLYGON_H

#include <vector>
#include <memory>
#include <functional>
#include <iostream>
#include <gdal/ogrsf_frmts.h>
#include "Point.h"
#include "Geometry.h"
#include "Ring.h"


class Polygon : public Geometry<Polygon>
{
    public:
        Polygon();
        Polygon(Ring externalRing, std::vector<Ring> internalRings);
        virtual ~Polygon();

        operator OGRGeometry*() const;
        Polygon& operator=(const OGRGeometry* geometry);

        Ring& ExternalRing();
        std::vector<Ring>& InternalRings();

        Ring GetExternalRing() const;
        std::vector<Ring> GetInternalRings() const;

        Polygon Transform(Geometry<Polygon>::TransformFunction transformFunction) const;
    private:
        Ring _externalRing;
        std::vector<Ring> _internalRings;
};



inline std::ostream& operator<<(std::ostream &strm, const Polygon& polygon)
{
    //TODO refactor Get methods.
    strm << "Polygon" << endl;
    strm << "==== External Ring ====" << endl;
    strm << polygon.GetExternalRing() << endl;
    for (auto& ring : polygon.GetInternalRings())
    {
        strm << "==== Internal Ring ====" << endl;
        strm << ring << endl;
    }
    return strm;
}

#endif
