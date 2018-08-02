#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <gdal/ogrsf_frmts.h>
#include <memory>
#include <functional>
#include <vector>
#include <tuple>
#include "SpatialReference.h"

class Geometry
{
    public:
        enum Type {PointType, RingType, PolygonType, MultiPolygonType, Other};

        using TransformFunction = std::function< std::vector<std::tuple<double, double> >(const std::vector<std::tuple<double, double>>)>;

    public:
        Geometry(const Type& geometryType);
        Geometry(const Type&, const SpatialReference& reference);
        virtual ~Geometry();

        virtual operator OGRGeometry*() const = 0;

        SpatialReference GetSpatialReference() const;
        void SetSpatialReference(const SpatialReference& spatialReference);
        Type GetType() const;

        virtual shared_ptr<Geometry> Transform(TransformFunction transformFunction) const = 0;

protected:
        void SetType(Type type);
        Type ParseGeometryType(OGRGeometry *geometry);

protected:
    SpatialReference _spatialReference;
    Type _type;
};

#endif // GEOMETRY_H
