#include "VectorFile.h"
#include <iostream>

VectorFile::VectorFile(string filename)
                :   _filename(filename),
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
        _dataset = (GDALDataset*) GDALOpenEx( Filename().c_str(), GDAL_OF_VECTOR, NULL, NULL, NULL );
    return _dataset;
}

int VectorFile::LayerCount()
{
    return Dataset()->GetLayerCount();
}

Layer& VectorFile::operator[](const int index)
{
    return Layers()[index];
}

Layer VectorFile::operator[](const char* layerName)
{
    return Layer(Dataset()->GetLayerByName(layerName));
}

vector<Layer>& VectorFile::Layers()
{
    if (_layers.size() == 0)
    {
        for (int i = 0; i < LayerCount(); i++)
        {
            Layer layer(Dataset()->GetLayer(i));
            _layers.push_back(layer);
        }
    }
    return _layers;
}


vector<Feature> VectorFile::ExecuteSql(const char* sqlStatement)
{
    OGRLayer* layer = Dataset()->ExecuteSQL(sqlStatement, nullptr, nullptr);
    
    vector<Feature> resultSetFeatures;
    Layer resultSet(layer);
    for (auto feature : resultSet)
    {
        resultSetFeatures.push_back(feature);
    }
    Dataset()->ReleaseResultSet(layer);
    return resultSetFeatures;
}
