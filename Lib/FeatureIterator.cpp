#include "FeatureIterator.h"


FeatureIterator::FeatureIterator(Layer* layer, bool start)
                    :   _layer(layer)
{
    if (start)
    {
        NextFeature();
    }
}

FeatureIterator::FeatureIterator(const FeatureIterator& iterator)
                    :   _layer(iterator._layer),
                        _currentFeature(iterator._currentFeature)
{
    //TODO: check whether unique_ptr can be copied here
}

FeatureIterator::~FeatureIterator()
{
}

FeatureIterator& FeatureIterator::operator=(const FeatureIterator& iterator)
{
    _layer = iterator._layer;
    return *this;
}

FeatureIterator::reference FeatureIterator::operator*() const
{
    return _currentFeature;
}

FeatureIterator& FeatureIterator::operator++()
{
    NextFeature();
    return *this;
}

FeatureIterator FeatureIterator::operator++(int)
{
    FeatureIterator temp = *this;
    ++(*this);
    return temp;
}

bool FeatureIterator::operator==(const FeatureIterator& rhs)
{
    return _currentFeature.get() == rhs._currentFeature.get();
}

bool FeatureIterator::operator!=(const FeatureIterator& rhs)
{
    return !(*this == rhs);
}

void FeatureIterator::NextFeature()
{
    _currentFeature.reset(_layer->NextFeature());
}
