#ifndef VECTOR_FILE_H
#define VECTOR_FILE_H

#include <string>
#include <vector>
#include <gdal_priv.h>
#include "Layer.h"

using namespace std;

class VectorFile
{
    public:
        VectorFile(string filename);
        virtual ~VectorFile();
    
        string Filename();

        int LayerCount();
        Layer& operator[](const int index);
        Layer operator[](const char* layerName);
        vector<Layer>& Layers();

    protected:
        GDALDataset* Dataset();

    private:
        string _filename;
        GDALDataset* _dataset;
        vector<Layer> _layers;
};

#endif
