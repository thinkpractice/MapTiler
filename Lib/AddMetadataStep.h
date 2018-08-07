#ifndef ADDMETADATASTEP_H
#define ADDMETADATASTEP_H

#include <string>
#include <memory>
#include "ProcessingStep.h"
#include "VectorFile.h"

using namespace std;

class AddMetadataStep : public ProcessingStep
{
public:
    AddMetadataStep(std::string metadataName, std::shared_ptr<VectorFile> vectorFile, int layerIndex = 0, std::string persistenceUrl = "", std::string persistenceLayerName = "");
    virtual ~AddMetadataStep();

    std::string MetadataName();
    int LayerIndex();
    std::shared_ptr<Layer> GetLayer();
    virtual void Run();

private:
    std::string _metadataName;
    std::shared_ptr<VectorFile> _vectorFile;
    int _layerIndex;
    std::string _persistenceUrl;
    std::string _persistenceLayerName;
};

#endif // ADDMETADATASTEP_H
