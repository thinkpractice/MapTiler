#ifndef FEATURE_ITERATOR_H
#define FEATURE_ITERATOR_H

#include <iterator>
#include "Layer.h"
#include "Feature.h"

using namespace std;

class FeatureIterator
{
    public:
        using value_type = Feature;
        using difference_type = ptrdiff_t;
        using pointer = Feature*;
        using reference = Feature&;
        using iterator_category = input_iterator_tag;

        FeatureIterator(Layer* layer, bool start);
        FeatureIterator(const FeatureIterator& iterator);
        virtual ~FeatureIterator();

        //Copy assignable
        FeatureIterator operator= (const FeatureIterator& iterator);

        // Dereferencable.
        reference operator*() const;

        // Pre- and post-incrementable.
        FeatureIterator& operator++();
        FeatureIterator operator++(int);

        // Equality / inequality.
        bool operator==(const FeatureIterator& rhs);
        bool operator!=(const FeatureIterator& rhs);
};

#endif
