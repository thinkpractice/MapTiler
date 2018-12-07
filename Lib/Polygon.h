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
#include "Rect.h"


class Polygon : public Geometry
{
    public:
        Polygon();
        Polygon(Ring externalRing, std::vector<Ring> internalRings);
        Polygon(const OGRGeometry* geometry);
        virtual ~Polygon();

        Rect BoundingBox();

        operator OGRGeometry*() const;
        Polygon& operator=(const OGRGeometry* geometry);

        Ring& ExternalRing();
        std::vector<Ring>& InternalRings();

        Ring GetExternalRing() const;
        std::vector<Ring> GetInternalRings() const;

        shared_ptr<Geometry> Transform(Geometry::TransformFunction transformFunction) const;

    private:
        void ParseGeometry(const OGRGeometry* geometry);

    private:
        Ring _externalRing;
        std::vector<Ring> _internalRings;
};

static GeometryTemplateFactory<Polygon> global_PolygonFactory(Geometry::PolygonType);

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
