#ifndef LAYER_H
#define LAYER_H

#include <string>
#include "ogrsf_frmts.h"
#include "SpatialReference.h"
#include "Feature.h"
#include "FeatureIterator.h"

using namespace std;

class Layer
{
    public:
        Layer(OGRLayer* layer);
        virtual ~Layer();

        string Name();
        SpatialReference ProjectionReference();

        void ResetReading();
        Feature* NextFeature();

        using iterator = FeatureIterator;

        iterator begin() const;
        iterator end() const;

        class FeatureIterator
        {
            public:
                using value_type = Feature;
                using difference_type = ptrdiff_t;
                using pointer = Feature*;
                using reference = shared_ptr<Feature*>&;
                using iterator_category = input_iterator_tag;

                FeatureIterator(Layer* layer, bool start);
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
                Layer* _layer;
                reference _currentFeature;
        };


    protected:
        OGRFeatureDefn* FeatureDefinition();

    private:
        OGRLayer* _layer;
};

#endif
