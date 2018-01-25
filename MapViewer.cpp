#include "gdal_priv.h"
#include "cpl_conv.h" // for CPLMalloc()
#include <iostream>
#include <regex>
#include <vector>
#include <string>

using namespace std;

struct Dataset
{
    string title;
    string url;
};

vector<string> splitKeyValuePair(const char* keyValueString)
{
    vector <string> matchResults;

    regex re(R"((\w+)=(.+))");
    cmatch matches;
    
    if (regex_match(keyValueString, matches, re))
    {
        for (const auto& matchResult : matches)
        {
            matchResults.push_back(string(matchResult));
        }
    }

    return matchResults;
}


string getKeyType(string key)
{
    regex re(R"((\w+_\d+_)(\w+))");
    smatch matches;
    if (regex_match(key, matches, re))
    {
        return matches[2];
    }
    return "";
}

int main()
{
    GDALAllRegister();

    const char *pszFilename = u8"WMTS:https://geodata.nationaalgeoregister.nl/luchtfoto/rgb/wmts/1.0.0/WMTSCapabilities.xml";
    GDALDataset  *wmtsDataset = (GDALDataset *) GDALOpen( pszFilename, GA_ReadOnly );
    if( wmtsDataset == NULL )
    {
        cout << "Error occurred" << endl;
        return -1;
    }

    char** metadata = wmtsDataset->GetMetadata("SUBDATASETS");

    vector<Dataset> datasets;
    if (metadata)
    {
        for (int i = 0; metadata[i]; i++)
        {
               vector<string> matches = splitKeyValuePair(metadata[i]);
               string key = matches[1];
               string value = matches[2];
               string keyType = getKeyType(key);
               Dataset dataset;
               if (keyType == "NAME")
               {
                   dataset.url = value;
                   datasets.push_back(dataset);
               }
               else if (keyType  == "DESC")
               {
                   Dataset& dataset = datasets.back();
                   dataset.title = value;
               }
        }

        int i = 0;
        for (auto& dataset : datasets)
        {
            cout << i << ") title=" << dataset.title << ", url=" << dataset.url << endl;
            i++;
        }

    }

    GDALDataset *poDataset = wmtsDataset;
    if (datasets.size() > 0)
    {
        cout << "Choose a dataset" << endl;
        string input;
        int datasetIndex = -1;
        do
        {
            getline(cin, input);
            datasetIndex = stoi(input);
        }
        while (datasetIndex >= 0 && datasetIndex < datasets.size());
        poDataset = (GDALDataset*)GDALOpen(datasets[datasetIndex].url.c_str(), GA_ReadOnly);
    }

     
    cout << "raster count=" << poDataset->GetRasterCount() << endl;
    cout << "layer count=" << poDataset->GetLayerCount() << endl;
    cout << "gcp count=" << poDataset->GetGCPCount() << endl;
    cout << "Get first raster band" << endl;
    GDALRasterBand* poBand = poDataset->GetRasterBand(1);
    cout << "poDataset = " << poDataset << "poBand = " << poBand << endl;
    
    if (!poBand)
    {
        cout << "poBand not initialized" << endl;
        return -1;
    }

    cout << "Get BlockSize" << endl;
    int nXBlockSize, nYBlockSize;
    poBand->GetBlockSize(&nXBlockSize, &nYBlockSize);

    cout << "Calculate number of blocks" << endl;
    int nXBlocks = (poBand->GetXSize() + nXBlockSize - 1) / nXBlockSize;
    int nYBlocks = (poBand->GetYSize() + nYBlockSize - 1) / nYBlockSize;
    
    cout << "blocksize (" << nXBlockSize << "," << nYBlockSize << ")";
    cout << "number of blocks (" << nXBlocks << "," << nYBlocks << ")";		

    for (int y = 0; y < nYBlocks; y++)
    {
        GByte* data = (GByte*)CPLMalloc(nXBlockSize * nYBlockSize);
        for (int x = 0; x < nXBlocks; x++)
        {
            CPLErr error = poBand->ReadBlock(x, y, data);
            if (error != CPLErr::CE_None)
                CPLFree(data);
                continue;
        }
        CPLFree(data);
    }

    return 0;
}
