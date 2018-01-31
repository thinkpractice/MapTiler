#include "GeoMapProvider.h"
#include "Utils.h"

#define kNameFieldKey "NAME"
#define kDescriptionFieldKey "DESC"

GeoMapProvider::GeoMapProvider(string filename)
                   :   _filename(filename)
{
}

GeoMapProvider::~GeoMapProvider()
{
    for (auto* map : _maps)
    {
        delete map;
    }
}

vector<GeoMap*> GeoMapProvider::Maps()
{
    if (_maps.size() == 0)
    {
        _maps = RetrieveMaps();
    }
    return _maps;
}

vector<GeoMap*> GeoMapProvider::RetrieveMaps()
{
    vector<GeoMap*> maps;

    GDALDataset  *dataset = (GDALDataset *) GDALOpen(_filename.c_str(), GA_ReadOnly );
    if(!dataset)
    {
        cout << "Error occurred" << endl;
        return maps;
    }

    char** metadata = dataset->GetMetadata("SUBDATASETS");
    if (!metadata)
    {
        maps.push_back(new GeoMap(_filename));
    }
    
    for (int i = 0; metadata[i]; i++)
    {
           vector<string> matches = Utils::SplitKeyValuePair(metadata[i]);
           string key = matches[1];
           string value = matches[2];
           string keyType = GetKeyType(key);
           if (keyType == kNameFieldKey)
           {
               maps.push_back(new GDALMap(value));
           }
           else if (keyType  == kDescriptionFieldKey)
           {
               GeoMap* map = maps.back();
               map->SetTitle(value);
           }
    }
    return maps;
}

