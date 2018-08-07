#ifndef FEATURE_H
#define FEATURE_H

#include <gdal/ogrsf_frmts.h>
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

        long long FeatureId() const;
        string Name() const;
        int NumberOfFields() const;

        Feature& operator=(const Feature& feature);
        Field operator[](size_t index);
        const Field operator[](size_t index) const;

        Field operator[](const char* fieldName);
        const Field operator[](const char* fieldName) const;

        bool operator==(const Feature& other) const;

        void SetField(const Field& field);
        void SetField(string fieldName, const Field& field);
        void SetField(string fieldName, string value);
        void SetField(string fieldName, int value);
        void SetField(string fieldName, double value);
        void SetField(string fieldName, long long value);

        void SetGeometry(const shared_ptr<Geometry> geometry)
        {
            _innerGeometry = geometry;

            SpatialReference sourceReference = geometry->GetSpatialReference();

            OGRGeomFieldDefn* geometryDefinition = FeatureDefinition()->GetGeomFieldDefn(0);
            SpatialReference destinationReference(geometryDefinition->GetSpatialRef());

            CoordinateTransformation transformation(sourceReference, destinationReference);
            _feature->SetGeometryDirectly(*transformation.MapGeometry(geometry));
        }

        std::shared_ptr<Geometry> GetGeometry() const;

        void MapGeometry(shared_ptr<CoordinateTransformation> transformation);
        void MapGeometry(shared_ptr<CoordinateTransformation> transformation, AffineTransform affineTransform);

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
                int _currentIndex;
                Field _currentField;
        };

        using iterator = FieldIterator;

        iterator begin() const;
        iterator end() const;

    private:
        Field GetFieldWithName(const char* name) const;
        Field GetFieldAtIndex(int index) const;
        OGRFeatureDefn *FeatureDefinition() const;

    private:
        OGRFeature* _feature;
        std::shared_ptr<Geometry> _innerGeometry;
};

#endif
