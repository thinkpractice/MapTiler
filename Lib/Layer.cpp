#include "Layer.h"
#include "CoordinateTransformation.h"
#include <iostream>
#include <algorithm>

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

SpatialReference Layer::ProjectionReference() const
{
    return SpatialReference(_layer->GetSpatialRef());
}

void Layer::ClearSpatialFilter()
{
    _layer->SetSpatialFilter(nullptr);
}

void Layer::SetSpatialFilter(const Area& area)
{
    ClearSpatialFilter();

    Area filterArea = CoordinateTransformation::MapArea(area, ProjectionReference());
    double minX = min(filterArea.LeftTop().X, filterArea.BottomRight().X);
    double minY = min(filterArea.LeftTop().Y, filterArea.BottomRight().Y);
    double maxX = max(filterArea.LeftTop().X, filterArea.BottomRight().X);
    double maxY = max(filterArea.LeftTop().Y, filterArea.BottomRight().Y);
    _layer->SetSpatialFilterRect(minX, minY, maxX, maxY);
}

Feature Layer::NewFeature()
{
    auto featureDefinition = FeatureDefinition();
    OGRFeature* feature = new OGRFeature(featureDefinition);
    return Feature(feature);
}

void Layer::AddFeature(const Feature &feature)
{
    auto error = _layer->CreateFeature(feature.InternalFeature());
    if (error != OGRERR_NONE)
        std::cerr << "Error adding feature: " << error << std::endl;
}

void Layer::Save()
{
    _layer->SyncToDisk();
}

void Layer::ResetReading() const
{
    _layer->ResetReading();
}

std::shared_ptr<Feature> Layer::NextFeature() const
{
    OGRFeature* feature = _layer->GetNextFeature();
    if (!feature)
        return nullptr;
    return make_shared<Feature>(feature);
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
                        _currentFeature(nullptr),
                        _lastFeature(true)
{
    if (start)
    {
        _layer->ResetReading();
        _lastFeature = false;
        NextFeature();
    }
}

Layer::FeatureIterator::FeatureIterator(const FeatureIterator& iterator)
                    :   _layer(iterator._layer),
                        _currentFeature(iterator._currentFeature),
                        _lastFeature(iterator._lastFeature)
{
}

Layer::FeatureIterator::~FeatureIterator()
{
}

Layer::FeatureIterator& Layer::FeatureIterator::operator=(const FeatureIterator& iterator)
{
    _layer = iterator._layer;
    _currentFeature = iterator._currentFeature;
    _lastFeature = iterator._lastFeature;
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
    return (_currentFeature == rhs._currentFeature);
}

bool Layer::FeatureIterator::operator!=(const FeatureIterator& rhs)
{
    return !_lastFeature;
}

void Layer::FeatureIterator::NextFeature()
{
    auto nextFeature = _layer->NextFeature();
    if (!nextFeature)
    {
        _lastFeature = true;
        return;
    }
    _currentFeature = *nextFeature;
}
