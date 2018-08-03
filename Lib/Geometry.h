#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <gdal/ogrsf_frmts.h>
#include <memory>
#include <functional>
#include <vector>
#include <tuple>
#include "SpatialReference.h"

class Geometry;

class GeometryFactory
{
    public:
        GeometryFactory();
        virtual ~GeometryFactory();

        virtual std::shared_ptr<Geometry> Create() = 0;
        virtual std::shared_ptr<Geometry> Create(OGRGeometry* geometry) = 0;
};

class Geometry
{
    public:
        enum Type {PointType, RingType, PolygonType, MultiPolygonType, Other};

        using TransformFunction = std::function< std::vector<std::tuple<double, double> >(const std::vector<std::tuple<double, double>>)>;

    public:
        Geometry(const Type& geometryType);
        Geometry(const Type& geometryType, const SpatialReference& reference);
        virtual ~Geometry();

        virtual operator OGRGeometry*() const = 0;

        SpatialReference GetSpatialReference() const;
        void SetSpatialReference(const SpatialReference& spatialReference);
        Type GetType() const;

        bool IsPoint();
        bool IsRing();
        bool IsPolygon();
        bool IsMultiPolygon();
        bool IsOther();

        virtual shared_ptr<Geometry> Transform(TransformFunction transformFunction) const = 0;

    public:
        static Type ParseGeometryType(OGRGeometry *geometry);

        static std::map<Type, GeometryFactory*>& GetFactoryRegistry()
        {
            static std::map<Type, GeometryFactory*> factories;
            return factories;
        }

        static void RegisterFactory(Type type, GeometryFactory* factory)
        {
            GetFactoryRegistry()[type] = factory;
        }

        static std::shared_ptr<Geometry> NewGeometry(Geometry::Type type)
        {
            return GetFactoryRegistry()[type]->Create();
        }

        static std::shared_ptr<Geometry> NewGeometry(OGRGeometry* geometry)
        {
            if (!geometry)
                return nullptr;

            auto type = Geometry::ParseGeometryType(geometry);
            return NewGeometry(type);
        }

    protected:
        void SetType(Type type);

    protected:
        SpatialReference _spatialReference;
        Type _type;

};

template <class T>
class GeometryTemplateFactory : public GeometryFactory
{
    public:
        GeometryTemplateFactory(Geometry::Type type)
        {
            Geometry::RegisterFactory(type, this);
        }

        virtual ~GeometryTemplateFactory()
        {
        }

        virtual std::shared_ptr<Geometry> Create()
        {
            return make_shared<T>();
        }

        virtual std::shared_ptr<Geometry> Create(OGRGeometry* geometry)
        {
            return make_shared<T>(geometry);
        }

};

#endif // GEOMETRY_H
