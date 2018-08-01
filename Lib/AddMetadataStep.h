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
    AddMetadataStep(string metadataName, shared_ptr<VectorFile> vectorFile, int layerIndex = 0, std::string persistenceUrl = "");
    virtual ~AddMetadataStep();

    string MetadataName();
    int LayerIndex();
    shared_ptr<Layer> GetLayer();
    virtual void Run();

private:
    string _metadataName;
    shared_ptr<VectorFile> _vectorFile;
    int _layerIndex;
    string _persistenceUrl;
};

#endif // ADDMETADATASTEP_H
