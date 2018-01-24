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
	return -1;
    }
    cout << "raster count=" << poDataset->GetRasterCount() << endl;
    cout << "layer count=" << poDataset->GetLayerCount() << endl;
    cout << "gcp count=" << poDataset->GetGCPCount() << endl;
    cout << "Get first raster band" << endl;
    GDALRasterBand* poBand = poDataset->GetRasterBand(1);
    cout << "poDataset = " << poDataset << "poBand = " << poBand << endl;

    char** metadata = poDataset->GetMetadata("SUBDATASETS");
    if (metadata)
    {
        for (int i = 0; metadata[i]; i++)
	{
	   cout << metadata[i] << endl;
	}
    }	

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
	    //int nXValid, nYValid;
	    //poBand->GetActualBlockSize(x, y, &nXValid, &nYValid); 
        }
	CPLFree(data);
    }

    return 0;
}
