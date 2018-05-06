#include "Feature.h"

Feature::Feature(OGRFeature* feature)
            :   _feature(feature)
{
}

Feature::~Feature()
{
    OGRFeature::DestroyFeature(_feature);
}
