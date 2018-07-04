#include "GeoMapProvider.h"
#include "Utils.h"
#include "GDALMap.h"
#include "gdal_priv.h"
#include <iostream>

#define kNameFieldKey "NAME"
#define kDescriptionFieldKey "DESC"

GeoMapProvider::GeoMapProvider(string filename)
					:	_filename(filename)
{
    GDALAllRegister();
}

GeoMapProvider::~GeoMapProvider()
{
}

vector< shared_ptr< GeoMap > > GeoMapProvider::Maps()
{
    if (_maps.size() == 0)
    {
        _maps = RetrieveMaps();
    }
    return _maps;
}

vector< shared_ptr< GeoMap > > GeoMapProvider::RetrieveMaps()
{
    vector<shared_ptr<GeoMap>> maps;

    GDALDataset  *dataset = (GDALDataset *) GDALOpen(_filename.c_str(), GA_ReadOnly );
    if(!dataset)
    {
        cout << "Error occurred" << endl;
        return maps;
    }

    char** metadata = dataset->GetMetadata("SUBDATASETS");
    if (!metadata)
    {
        maps.push_back(make_shared<GDALMap>(_filename));
    }
    else
    {
        for (int i = 0; metadata[i]; i++)
        {
            vector<string> matches = Utils::SplitKeyValuePair(metadata[i]);
            string key = matches[1];
            string value = matches[2];
            string keyType = Utils::GetKeyType(key);
            if (keyType == kNameFieldKey)
            {
                maps.push_back(make_shared<GDALMap>(value));
            }
            else if (keyType  == kDescriptionFieldKey)
            {
                auto map = maps.back();
                map->SetTitle(value);
            }
        }
    }
    GDALClose(dataset);
    return maps;
}

