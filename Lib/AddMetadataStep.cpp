#include "AddMetadataStep.h"
#include "DatabaseWrapper.h"

AddMetadataStep::AddMetadataStep(std::string metadataName, std::shared_ptr<VectorFile> vectorFile, int layerIndex, std::string persistenceUrl, std::string persistenceLayerName)
                    :	ProcessingStep(Processing),
                        _metadataName(metadataName),
                        _vectorFile(vectorFile),
                        _layerIndex(layerIndex),
                        _persistenceUrl(persistenceUrl),
                        _persistenceLayerName(persistenceLayerName)
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

shared_ptr<Layer> AddMetadataStep::GetLayer()
{
    if (LayerIndex() >= 0)
        return _vectorFile->Layers()[LayerIndex()];
    return _vectorFile->operator[](MetadataName().c_str());
}

void AddMetadataStep::Run()
{
    auto layer = GetLayer();
    shared_ptr<DatabaseWrapper> databasePersistence = DatabaseWrapper::DatabaseWrapperFor(_persistenceUrl);
    while (auto stepData = InQueue()->dequeue())
    {
        layer->SetSpatialFilter(stepData->BoundingArea());
        vector<Feature> metadataFeatures;
        for (auto it = layer->begin(); it != layer->end(); ++it)
        {
            auto feature = *it;
            if (databasePersistence) databasePersistence->SaveMetadata(_persistenceLayerName, stepData->TileId(), feature);
            metadataFeatures.push_back(feature);
        }
        stepData->AddMetadataFeatures(_metadataName, metadataFeatures);
        OutQueue()->enqueue(std::move(stepData));
    }
    DoneProcessing();
}
