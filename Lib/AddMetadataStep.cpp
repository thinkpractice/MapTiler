#include "AddMetadataStep.h"

AddMetadataStep::AddMetadataStep(string metadataName, string metadataUrl, int layerIndex)
                    :	ProcessingStep(PreProcessing),
                        _metadataName(metadataName),
                        _metadataUrl(metadataUrl),
                        _vectorFile(_metadataUrl),
                        _layerIndex(layerIndex)
{
}

AddMetadataStep::~AddMetadataStep()
{
}

string AddMetadataStep::MetadataName()
{
    return _metadataName;
}

string AddMetadataStep::MetadataUrl()
{
    return _metadataUrl;
}

int AddMetadataStep::LayerIndex()
{
    return _layerIndex;
}

void AddMetadataStep::Run()
{
    auto layer = _vectorFile[_layerIndex];
    while (auto stepData = InQueue()->dequeue())
    {
        layer->SetSpatialFilter(stepData->BoundingArea());

        vector<Feature> metadataFeatures;
        for (auto it = layer->begin(); it != layer->end(); ++it)
        {
            auto feature = *it;
            metadataFeature.push_back(feature);
        }
        stepData->AddMetadataFeatures(_metadataName, metadataFeatures);
    }
}
