#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "ogrsf_frmts.h"
#include <memory>
#include <vector>
#include "SpatialReference.h"

template <class T>
class Geometry
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

    SpatialReference GetSpatialReference() const
    {
        return _spatialReference;
    }

    void SetSpatialReference(const SpatialReference& spatialReference)
    {
        _spatialReference = spatialReference;
    }

protected:
    SpatialReference _spatialReference;
};

#endif // GEOMETRY_H
