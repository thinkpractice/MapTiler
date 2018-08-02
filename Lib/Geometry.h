#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <gdal/ogrsf_frmts.h>
#include <memory>
#include <functional>
#include <vector>
#include "SpatialReference.h"

struct Point;

class BaseGeometry
{
    public:
        enum Type {PointType, PolygonType, MultiPolygonType, Other};

    public:
        BaseGeometry();
        BaseGeometry(const SpatialReference& reference);
        virtual ~BaseGeometry();

        SpatialReference GetSpatialReference() const;
        void SetSpatialReference(const SpatialReference& spatialReference);
        Type GetType() const;

protected:
        void SetType(Type type);
        Type ParseGeometryType(OGRGeometry *geometry);

protected:
    SpatialReference _spatialReference;
    Type _type;
};

template <class T>
class Geometry : public BaseGeometry
{
public:

    using TransformFunction = std::function< std::vector<Point>(const std::vector<Point>)>;

    Geometry()
        :	BaseGeometry()
    {
    }

    Geometry(const SpatialReference& spatialReference)
                :	BaseGeometry(spatialReference)
    {
    }

    virtual ~Geometry()
    {
    }

    virtual operator OGRGeometry*() const = 0;
    virtual T Transform(Geometry<T>::TransformFunction transformFunction) const = 0;

};

#endif // GEOMETRY_H
