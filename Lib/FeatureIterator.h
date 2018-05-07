#ifndef FEATURE_ITERATOR_H
#define FEATURE_ITERATOR_H

#include <iterator>
#include <memory>
#include "Feature.h"

class Layer;

using namespace std;

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

#endif
