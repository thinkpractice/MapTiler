#include "GDALMap.h"
#include <iostream>

GDALMap::GDALMap(string filename)
            :   GeoMap(filename),
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
    return Dataset()->GetLayerCount();
}

int GDALMap::RasterCount()
{
    return Dataset()->GetRasterCount();
}

int GDALMap::WidthInPixels()
{
    return Dataset()->GetRasterXSize();
}

int GDALMap::HeightInPixels()
{
    return Dataset()->GetRasterYSize();
}

SpatialReference GDALMap::ProjectionReference()
{
    return SpatialReference(Dataset()->GetProjectionRef());
}

AffineTransform GDALMap::MapTransform()
{
    double transform[6];
    if (Dataset()->GetGeoTransform(transform) != CPLErr::CE_None)
        cout << "Error getting geotransform" << endl;
    return AffineTransform(transform);
}

Area GDALMap::GetMapArea()
{
    double transform[6];
    MapTransform().GetTransformMatrix(transform);
    Point topRight = MapTransform().Transform(Point(WidthInPixels(), HeightInPixels()));

    double minX = transform[0];
    double maxX = topRight.X;
    double minY = topRight.Y;
    double maxY = transform[3];
    return Area(ProjectionReference(), Point(minX, minY), Point(maxX, maxY));
}

GeoTile* GDALMap::GetTileForRect(const Rect& rectangle)
{
    int width, height = 0;
    tie(width, height) = GetTileSize();
    int numberOfTilePixels = width*height;
    int arrayLength = numberOfTilePixels * RasterCount();

    GByte *rasterData[RasterCount()];
    for (int i = 0; i < RasterCount(); i++)
    {
        rasterData[i] = GetDataForBand(i, rectangle.Left(), rectangle.Top(), rectangle.Width(), rectangle.Height());
    }

    Area tileArea = AreaForRect(rectangle);
    GeoTile* geoTile = new GeoTile(rectangle, tileArea, RasterCount());
    geoTile->SetRasterData(rasterData);

    for (int i = 0; i < RasterCount();i++)
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
    Point rasterLeftTop = rect.LeftTop();
    Point rasterRightBottom = rect.RightBottom();

    Point leftTop = RasterToProjectionCoord(rasterLeftTop);
    Point rightBottom = RasterToProjectionCoord(rasterRightBottom);
    return Area(ProjectionReference(), leftTop, rightBottom);
}

GDALDataset* GDALMap::Dataset()
{
    if (!_dataset)
        _dataset = (GDALDataset*)GDALOpen(Filename().c_str(), GA_ReadOnly);
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

Point GDALMap::RasterToProjectionCoord(Point rasterCoord)
{
    return MapTransform().Transform(rasterCoord);
}

