#include "gdal_priv.h"
#include "cpl_conv.h" // for CPLMalloc()
#include <iostream>

using namespace std;

int main()
{
    GDALDataset  *poDataset;
    GDALAllRegister();
    const char *pszFilename = u8"WMTS:https://geodata.nationaalgeoregister.nl/luchtfoto/rgb/wmts/1.0.0/WMTSCapabilities.xml";
    poDataset = (GDALDataset *) GDALOpen( pszFilename, GA_ReadOnly );
    if( poDataset == NULL )
    {
        cout << "Error occurred" << endl;
    }
}
