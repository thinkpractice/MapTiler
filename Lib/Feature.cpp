#include "Feature.h"

Feature::Feature(OGRFeature* feature)
            :   _feature(feature)
{
}

Feature::~Feature()
{
    OGRFeature::DestroyFeature(_feature);
}

string Feature::Name()
{
    return string(FeatureDefinition()->GetName());
}

OGRFeatureDefn* Feature::FeatureDefinition()
{
    return _feature->GetDefnRef();
}
