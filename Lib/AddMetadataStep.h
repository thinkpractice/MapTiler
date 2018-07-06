#ifndef ADDMETADATASTEP_H
#define ADDMETADATASTEP_H

#include <string>
#include "ProcessingStep.h"
#include "VectorFile.h"

using namespace std;

class AddMetadataStep : public ProcessingStep
{
public:
    AddMetadataStep(string metadataName, string metadataUrl, int layerIndex = 0);
    virtual ~AddMetadataStep();

    string MetadataName();
    string MetadataUrl();
    int LayerIndex();
    virtual void Run();

private:
    string _metadataName;
    string _metadataUrl;
    VectorFile _vectorFile;
    int _layerIndex;
};

#endif // ADDMETADATASTEP_H
