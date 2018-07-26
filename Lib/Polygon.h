#ifndef POLYGON_H
#define POLYGON_H

#include <vector>
#include <memory>
#include <functional>
#include <iostream>
#include "ogrsf_frmts.h"
#include "Point.h"
#include "Geometry.h"
#include "Ring.h"

using namespace std;

class Polygon : public Geometry
{
    public:
        Polygon();
        Polygon(Ring externalRing, vector<Ring> internalRings);
        virtual ~Polygon();

        operator OGRGeometry*() const {};
        Polygon& operator=(const OGRGeometry* geometry);

        Ring& ExternalRing();
        vector<Ring>& InternalRings();

        Ring GetExternalRing() const;
        vector<Ring> GetInternalRings() const;

        Polygon Transform(Ring::TransformFunction transformFunction);
    private:
        Ring _externalRing;
        vector<Ring> _internalRings;
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
