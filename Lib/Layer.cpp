#include "Layer.h"
#include "CoordinateTransformation.h"

Layer::Layer(OGRLayer* layer)
            :   _layer(layer)
{
}

Layer::~Layer()
{
}

string Layer::Name()
{
    return string(_layer->GetName());
}

SpatialReference Layer::ProjectionReference()
{
    return SpatialReference(_layer->GetSpatialRef());
}

void Layer::ClearSpatialFilter()
{
    _layer->SetSpatialFilter(nullptr);
}

void Layer::SetSpatialFilter(const Area& area)
{

    Area filterArea = CoordinateTransformation::MapArea(area, ProjectionReference());
    _layer->SetSpatialFilterRect(filterArea.LeftTop().X, filterArea.LeftTop().Y, filterArea.BottomRight().X, filterArea.BottomRight().Y);
}

void Layer::ResetReading() const
{
    _layer->ResetReading();
}

Feature Layer::NextFeature() const
{
    OGRFeature* feature = _layer->GetNextFeature();
    return Feature(feature);
}

Layer::iterator Layer::begin() const
{
    return {this, true};
}

Layer::iterator Layer::end() const
{
    return {this, false};
}

OGRFeatureDefn* Layer::FeatureDefinition()
{
    return _layer->GetLayerDefn();
}

Layer::FeatureIterator::FeatureIterator(const Layer* layer, bool start)
                    :   _layer(layer),
                        _currentFeature(nullptr)
{
    if (start)
    {
        _layer->ResetReading();
        NextFeature();
    }
}

Layer::FeatureIterator::FeatureIterator(const FeatureIterator& iterator)
                    :   _layer(iterator._layer),
                        _currentFeature(iterator._currentFeature)
{
}

Layer::FeatureIterator::~FeatureIterator()
{
}

Layer::FeatureIterator& Layer::FeatureIterator::operator=(const FeatureIterator& iterator)
{
    _layer = iterator._layer;
    return *this;
}

Layer::FeatureIterator::reference Layer::FeatureIterator::operator*() const
{
    return _currentFeature;
}

Layer::FeatureIterator& Layer::FeatureIterator::operator++()
{
    NextFeature();
    return *this;
}

Layer::FeatureIterator Layer::FeatureIterator::operator++(int)
{
    FeatureIterator temp = *this;
    ++(*this);
    return temp;
}

bool Layer::FeatureIterator::operator==(const FeatureIterator& rhs)
{
    return _currentFeature == rhs._currentFeature;
}

bool Layer::FeatureIterator::operator!=(const FeatureIterator& rhs)
{
    return !(*this == rhs);
}

void Layer::FeatureIterator::NextFeature()
{
    _currentFeature = _layer->NextFeature();
}
