#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <gdal/ogrsf_frmts.h>
#include <memory>
#include <functional>
#include <vector>
#include "SpatialReference.h"

class Point;

class BaseGeometry
{
    public:
        enum Type {PointType, PolygonType, MultiPolygonType, Other};

    public:
        virtual ~BaseGeometry() = default;

        SpatialReference GetSpatialReference() const
        {
        return _spatialReference;
        }

        void SetSpatialReference(const SpatialReference& spatialReference)
        {
        _spatialReference = spatialReference;
        }

        Type GetType() const
        {
        return _type;
        }

protected:
        void SetType(Type type) { _type = type; }

        Type ParseGeometryType(OGRGeometry *geometry)
        {
            switch(wkbFlatten(geometry->getGeometryType()))
            {
                case wkbPoint:
                return PointType;
                case wkbPolygon:
                return PolygonType;
                case wkbMultiPolygon:
                return MultiPolygonType;
            }
            return Other;
        }

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
    {
    }

    Geometry(const SpatialReference& spatialReference)
                :	_spatialReference(spatialReference)
    {
    }

    virtual ~Geometry()
    {
    }

    virtual operator OGRGeometry*() const = 0;
    virtual T Transform(Geometry<T>::TransformFunction transformFunction) const = 0;

};

#endif // GEOMETRY_H
