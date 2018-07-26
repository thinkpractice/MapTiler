#include "VectorFile.h"
#include <iostream>

VectorFile::VectorFile(string filename, OpenMode mode)
                :   _filename(filename),
                    _mode(mode),
                    _dataset(nullptr)
{
}

VectorFile::~VectorFile()
{
    if (_dataset)
        GDALClose(_dataset);
}

string VectorFile::Filename()
{
    return _filename;
}

GDALDataset* VectorFile::Dataset()
{
    if (!_dataset)
        _dataset = (GDALDataset*) GDALOpenEx( Filename().c_str(), GDAL_OF_VECTOR | GDAL_OF_VERBOSE_ERROR | _mode, NULL, NULL, NULL );
    return _dataset;
}

int VectorFile::LayerCount()
{
    return Dataset()->GetLayerCount();
}

shared_ptr<Layer> VectorFile::operator[](const int index)
{
    return Layers()[index];
}

shared_ptr<Layer> VectorFile::operator[](const char* layerName)
{
    return LayerFor(Dataset()->GetLayerByName(layerName));
}

vector<shared_ptr<Layer>>& VectorFile::Layers()
{
    if (_layers.size() == 0)
    {
        for (int i = 0; i < LayerCount(); i++)
        {
             auto layer = LayerFor(Dataset()->GetLayer(i));
            _layers.push_back(layer);
        }
    }
    return _layers;
}

vector<Feature> VectorFile::ExecuteSql(const char* sqlStatement)
{
    OGRLayer* layer = Dataset()->ExecuteSQL(sqlStatement, nullptr, nullptr);
    
    vector<Feature> resultSetFeatures;
    auto resultSet = LayerFor(layer);
    for (auto feature : (*resultSet))
    {
        resultSetFeatures.push_back(feature);
    }
    Dataset()->ReleaseResultSet(layer);
    return resultSetFeatures;
}

shared_ptr<Layer> VectorFile::LayerFor(OGRLayer* layer)
{
    return make_shared<Layer>(layer);
}
