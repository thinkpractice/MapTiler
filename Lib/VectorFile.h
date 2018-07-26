#ifndef VECTOR_FILE_H
#define VECTOR_FILE_H

#include <string>
#include <vector>
#include <memory>
#include <gdal_priv.h>
#include "Layer.h"
#include "Feature.h"

using namespace std;

class VectorFile
{
    public:
        enum OpenMode
        {
            ReadOnly = GDAL_OF_READONLY,
            Update = GDAL_OF_UPDATE
        };

        VectorFile(string filename, OpenMode mode = ReadOnly);
        virtual ~VectorFile();
    
        string Filename();

        int LayerCount();
        shared_ptr<Layer> operator[](const int index);
        shared_ptr<Layer> operator[](const char* layerName);
        vector<shared_ptr<Layer>>& Layers();

        vector<Feature> ExecuteSql(const char* sqlStatement);

    protected:
        GDALDataset* Dataset();
        virtual shared_ptr<Layer> LayerFor(OGRLayer* layer);

    private:
        string _filename;
        GDALDataset* _dataset;
        OpenMode _mode;
        vector<shared_ptr<Layer>> _layers;
};

#endif
