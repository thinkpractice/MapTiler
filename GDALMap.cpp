#include "GDALMap.h"
#include <iostream>

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

GeoTile* GDALMap::GetTileForRect(const Rect& rectangle, const Area& area)
{
    int width, height = 0;
    tie(width, height) = GetTileSize();
    int numberOfTilePixels = width*height;
    int arrayLength = numberOfTilePixels * LayerCount();

    GByte *rasterData[LayerCount()];
    for (int i = 0; i < LayerCount(); i++)
    {
        rasterData[i] = GetDataForBand(i, rectangle.X(), rectangle.Y(), rectangle.Width(), rectangle.Height());
    }

    GeoTile* geoTile = new GeoTile(rectangle, area, LayerCount());
    geoTile->SetRasterData(rasterData);

    for (int i = 0; i < LayerCount();i++)
    {
        CPLFree(rasterData[i]);
    }

    return geoTile;
}

Rect GDALMap::RectForArea(const Area& area)
{
    return Rect(0, 0, 0, 0);
}

Area GDALMap::AreaForRect(const Rect& rect)
{
}

GDALDataset* GDALMap::Dataset()
{
    if (!_dataset)
        _dataset = (GDALDataset*)GDALOpen(_filename.c_str(), GA_ReadOnly);
    return _dataset;
}

tuple<int, int> GDALMap::GetTileSize()
{
    int width, height = 0;
    GDALRasterBand* band = Dataset()->GetRasterBand(0);
    band->GetBlockSize(&width, & height);
    return make_tuple(width, height);
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
