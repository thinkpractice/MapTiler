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
    AddMetadataStep(string metadataName, shared_ptr<VectorFile> vectorFile, int layerIndex = 0);
    virtual ~AddMetadataStep();

    string MetadataName();
    int LayerIndex();
    virtual void Run();

private:
    string _metadataName;
    shared_ptr<VectorFile> _vectorFile;
    int _layerIndex;
};

#endif // ADDMETADATASTEP_H
