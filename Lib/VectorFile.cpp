#include "VectorFile.h"

VectorFile::VectorFile(string filename)
                :   _filename(filename),
                    _dataset(nullptr)
{
}

VectorFile::~VectorFile()
{
    if (_dataset)
        GDALClose(_dataset);

    for (auto* layer : _layers)
        delete layer;
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


Layer* VectorFile::operator[](const int index)
{
    //TODO: convert to unique or shared ptr?
    return new Layer(Dataset()->GetLayer(index));
}

Layer* VectorFile::operator[](const char* layerName)
{
    //TODO: convert to unique or shared ptr?
    return new Layer(Dataset()->GetLayerByName(layerName));
}

vector<Layer*> VectorFile::Layers()
{
    if (_layers.size() == 0)
    {
        for (int i = 0; i < LayerCount(); i++)
        {
            _layers.push_back((*this)[i]);
        }
    }
    return _layers;
}
