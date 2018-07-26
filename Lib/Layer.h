#ifndef LAYER_H
#define LAYER_H

#include <string>
#include <memory>
#include "ogrsf_frmts.h"
#include "SpatialReference.h"
#include "Feature.h"
#include "Area.h"

using namespace std;

class Layer
{
    public:
        Layer(OGRLayer* layer);
        virtual ~Layer();

        string Name();
        SpatialReference ProjectionReference() const;

        void ClearSpatialFilter();
        void SetSpatialFilter(const Area& area);

        Feature NewFeature();
        void AddFeature(const Feature& feature);

        class FeatureIterator
        {
            public:
                using value_type = Feature;
                using difference_type = ptrdiff_t;
                using pointer = Feature*;
                using reference = const Feature&;
                using iterator_category = input_iterator_tag;

                FeatureIterator(const Layer* layer, bool start);
                FeatureIterator(const FeatureIterator& iterator);
                virtual ~FeatureIterator();

                //Copy assignable
                FeatureIterator& operator= (const FeatureIterator& iterator);

                // Dereferencable.
                reference operator*() const;

                // Pre- and post-incrementable.
                FeatureIterator& operator++();
                FeatureIterator operator++(int);

                // Equality / inequality.
                bool operator==(const FeatureIterator& rhs);
                bool operator!=(const FeatureIterator& rhs);

            private:
                void NextFeature();

            private:
                const Layer* _layer;
                Feature _currentFeature;
        };

        using iterator = FeatureIterator;

        iterator begin() const;
        iterator end() const;

    protected:
        OGRFeatureDefn* FeatureDefinition();

        void ResetReading() const;
        virtual Feature NextFeature() const;

    private:
        OGRLayer* _layer;
};

#endif
