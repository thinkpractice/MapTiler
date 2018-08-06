#include "AddMetadataStep.h"
#include "DatabaseWrapper.h"

AddMetadataStep::AddMetadataStep(string metadataName, shared_ptr<VectorFile> vectorFile, int layerIndex, std::string persistenceUrl)
                    :	ProcessingStep(Processing),
                        _metadataName(metadataName),
                        _vectorFile(vectorFile),
                        _layerIndex(layerIndex),
                        _persistenceUrl(persistenceUrl)
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
            if (databasePersistence) databasePersistence->SaveBuilding(stepData->TileId(), feature);
            metadataFeatures.push_back(feature);
        }
        stepData->AddMetadataFeatures(_metadataName, metadataFeatures);
        OutQueue()->enqueue(stepData);
    }
    DoneProcessing();
}
