#include "Layer.h"

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

void Layer::ResetReading()
{
    _layer->ResetReading();
}

Feature* Layer::NextFeature()
{
    OGRFeature* feature = _layer->GetNextFeature();
    if (feature)
        return new Feature(feature);
    return nullptr;
}

Layer::iterator Layer::begin() const
{
    return FeatureIterator(this, true);
}

Layer::iterator Layer::end() const
{
    return FeatureIterator(this, false);
}

OGRFeatureDefn* Layer::FeatureDefinition()
{
    return _layer->GetLayerDefn();
}

