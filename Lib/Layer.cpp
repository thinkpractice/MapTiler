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

OGRFeatureDefn* Layer::FeatureDefinition()
{
    return _layer->GetLayerDefn();
}
