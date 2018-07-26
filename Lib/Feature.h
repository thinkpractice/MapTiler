#ifndef FEATURE_H
#define FEATURE_H

#include "ogrsf_frmts.h"
#include <string>
#include <iterator>
#include <memory>
#include "Field.h"
#include "Point.h"
#include "Polygon.h"
#include "MultiPolygon.h"
#include "CoordinateTransformation.h"
#include "AffineTransform.h"

using namespace std;

class Feature
{
    public:
        Feature(OGRFeature *feature);
        Feature(const Feature& other);
        virtual ~Feature();

        OGRFeature* InternalFeature() const;

        long long FeatureId();
        string Name() const;
        size_t NumberOfFields() const;

        Feature& operator=(const Feature& feature);
        Field operator[](size_t index);
        const Field operator[](size_t index) const;

        bool operator==(const Feature& other) const;

        void SetField(const Field& field);
        void SetField(string fieldName, string value);
        void SetField(string fieldName, int value);
        void SetField(string fieldName, double value);
        void SetGeometry(const Geometry& geometry);

        class FeatureGeometry
        {
            public:
                enum GeometryType {PointType, PolygonType, MultiPolygonType, Other};

            public:
                FeatureGeometry();
                FeatureGeometry(OGRGeometry* geometry);
                virtual ~FeatureGeometry();

                GeometryType Type();

                bool HasPoint() const;
                Point GetPoint() const;

                bool HasPolygon() const;
                Polygon GetPolygon() const;

                bool HasMultiPolygon() const;
                MultiPolygon GetMultiPolygon() const;

                void MapGeometry(shared_ptr<CoordinateTransformation> transformation);

                void MapGeometry(shared_ptr<CoordinateTransformation> transformation, AffineTransform affineTransform);
            private:
                void ParseGeometry(OGRGeometry* geometry);
                Polygon ParsePolygon(OGRPolygon* polygon);
                GeometryType ParseGeometryType(OGRGeometry* geometry);

            private:
                OGRGeometry* _geometry;

                GeometryType _geometryType;
                bool _parsedGeometry;
                bool _hasPoint;
                Point _point;

                Polygon _polygon;
                bool _hasPolygon;

                MultiPolygon _multiPolygon;
                bool _hasMultiPolygon;
        };

        class FieldIterator
        {
            public:
                using value_type = Field;
                using difference_type = ptrdiff_t;
                using pointer = Field*;
                using reference = const Field&;
                using iterator_category = input_iterator_tag;

                FieldIterator(const Feature* owner, bool start);
                FieldIterator(const FieldIterator& iterator);
                virtual ~FieldIterator();

                FieldIterator& operator=(const FieldIterator& iterator);

                reference operator*() const;

                FieldIterator& operator++();
                FieldIterator operator++(int);

                bool operator==(const FieldIterator& rhs);
                bool operator!=(const FieldIterator& rhs);

            private:
                void NextField();

            private:
                const Feature* _owner;
                size_t _currentIndex;
                Field _currentField;
        };

        using iterator = FieldIterator;

        iterator begin() const;
        iterator end() const;
        
        FeatureGeometry& GetGeometry();

    private:
        Field GetFieldAtIndex(size_t index) const;
        OGRFeatureDefn *FeatureDefinition() const;

    private:
        OGRFeature* _feature;
        FeatureGeometry _featureGeometry;
        
};

#endif
