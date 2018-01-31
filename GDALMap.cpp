#include "GDALMap.h"

GDALMap::GDALMap(string filename)
            :   _filename(filename),
                _dataset(nullptr)
{
}

GDALMap::~GDALMap()
{
    if (_dataset)
        GDALClose(_dataset);
}

int GDALMap::LayerCount()
{
    Dataset()->GetLayerCount();
}

int GDALMap::WidthInPixels()
{
    return Dataset()->GetRasterXSize();
}

int GDALMap::HeightInPixels()
{
    return Dataset()->GetRasterYSize();
}

GByte* GDALMap::GetTile(int x, int y)
{

}

GDALDataset* GDALMap::Dataset()
{
    if (!_dataset)
        _dataset = (GDALDataset*)GDALOpen(filename);
    return _dataset;
}

GByte* GDALMap::GetDataForBand(int rasterIndex, int x, int y, int width, int height)
{
    GDALRasterBand* band = Dataset()->GetRasterBand(rasterIndex);
     
    printf( "Type=%s, ColorInterp=%s\n",
    GDALGetDataTypeName(band->GetRasterDataType()),
    GDALGetColorInterpretationName(
            band->GetColorInterpretation()) );

    GByte* data = (GByte*)CPLMalloc(width * height);

    //CPLErr error = band->ReadBlock(x, y, data);
    CPLErr error = band->RasterIO(GDALRWFlag::GF_Read, x, y, width, height, data, width, height, GDALDataType::GDT_Byte,0,0);
    if (error != CPLErr::CE_None)
    {
        cout << "error=" << error << endl;
        CPLFree(data);
        return nullptr;
    }
    return data;
}
