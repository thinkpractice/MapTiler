#include "AddMetadataStep.h"

AddMetadataStep::AddMetadataStep(string metadataName, shared_ptr<VectorFile> vectorFile, int layerIndex)
                    :	ProcessingStep(Processing),
                        _metadataName(metadataName),
                        _vectorFile(vectorFile),
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

int AddMetadataStep::LayerIndex()
{
    return _layerIndex;
}

void AddMetadataStep::Run()
{
    auto layer = _vectorFile->Layers()[_layerIndex];
    while (auto stepData = InQueue()->dequeue())
    {
        layer->SetSpatialFilter(stepData->BoundingArea());

        vector<Feature> metadataFeatures;
        for (auto it = layer->begin(); it != layer->end(); ++it)
        {
            auto feature = *it;
            metadataFeatures.push_back(feature);
        }
        stepData->AddMetadataFeatures(_metadataName, metadataFeatures);
        OutQueue()->enqueue(stepData);
    }
    DoneProcessing();
}
