#include "GDALMap.h"
#include <iostream>
#include <cstring>
#include "CoordinateTransformation.h"

GDALMap::GDALMap(std::string filename)
            :   GDALMap(filename, nullptr)
{
}

GDALMap::GDALMap(std::string filename, GDALDataset* dataset)
            :   GeoMap(filename),
                _dataset(dataset)
{
}

GDALMap::~GDALMap()
{
    if (_dataset)
        GDALClose(_dataset);
}

GeoMap* GDALMap::Clone()
{
    return new GDALMap(Filename());
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

void GDALMap::SetProjectionReference(const SpatialReference& reference)
{
    Dataset()->SetProjection(reference.ToWkt().c_str());
}

AffineTransform GDALMap::MapTransform()
{
    double transform[6];
    if (Dataset()->GetGeoTransform(transform) != CPLErr::CE_None)
    {
        cout << "Error getting geotransform" << endl;
        return AffineTransform();
    }
    return AffineTransform::FromGdal(transform);
}

void GDALMap::SetMapTransform(const AffineTransform& affineTransform)
{
    double transform[6];
    affineTransform.ToGdal(transform);
    Dataset()->SetGeoTransform(transform);
}

Area GDALMap::GetMapArea()
{
    double transform[6];
    MapTransform().ToGdal(transform);
    Point topRight = MapTransform().Transform(Point(WidthInPixels(), HeightInPixels()));

    double minX = transform[0];
    double maxX = topRight.X;
    double minY = topRight.Y;
    double maxY = transform[3];
    return Area(ProjectionReference(), Point(minX, minY), Point(maxX, maxY));
}

unique_ptr<GeoTile> GDALMap::GetTileForRect(const Rect& rectangle)
{
    int width, height = 0;
    tie(width, height) = GetTileSize();

    //Make sure images with less bands are converted to RGBA
    GByte *rasterData[4];
    for (int i = 0; i < RasterCount(); i++)
    {
		//cout << "Getting Rectangle " << rectangle.Left() << "," << rectangle.Top() << "," << rectangle.Width() << "," << rectangle.Height() << endl;
        rasterData[i] = GetDataForBand(i+1, static_cast<int>(rectangle.Left()), static_cast<int>(rectangle.Top()), rectangle.IntegerWidth(), rectangle.IntegerHeight());
    }

    //Add extra channels if not present
    int numberOfBandsToFill = 4 - RasterCount();
    if (numberOfBandsToFill > 0)
    {
        for (int i = 0; i < numberOfBandsToFill; i++)
        {
            size_t rasterBandSize = static_cast<size_t>(rectangle.IntegerWidth() * rectangle.IntegerHeight());
            GByte* data = static_cast<GByte*>(CPLMalloc(rasterBandSize));

            //fill alpha channel with 255, the rest of the channels with 0
            GByte fillValue = i == (numberOfBandsToFill - 1) ? 255 : 0;
            std::memset(data, fillValue, rasterBandSize);

            rasterData[RasterCount() + i] = data;
        }
    }

    Area tileArea = AreaForRect(rectangle);
    auto geoTile = make_unique<GeoTile>(rectangle, tileArea, 4);
    geoTile->SetRasterData(rasterData);

    for (int i = 0; i < 4;i++)
    {
        CPLFree(rasterData[i]);
    }

    return geoTile;
}

void GDALMap::WriteTile(shared_ptr<GeoTile> tile)
{
    int tileWidth = tile->BoundingRect().IntegerWidth();
    int tileHeight = tile->BoundingRect().IntegerHeight();
    for (int rasterIndex = 1; rasterIndex <= RasterCount(); rasterIndex++)
    {
        GDALRasterBand* rasterBand = Dataset()->GetRasterBand(rasterIndex);
        unsigned char* rasterData = tile->GetRasterBand(rasterIndex);
        rasterBand->RasterIO(GF_Write, 0, 0, tileWidth, tileHeight, rasterData, tileWidth, tileHeight, GDT_Byte, 0, 0 );
        delete[] rasterData;
    }
}

Rect GDALMap::RectForArea(const Area& area)
{
    Area mappedArea = CoordinateTransformation::MapArea(area, ProjectionReference());

    Point leftTop = ProjectionToRasterCoord(mappedArea.LeftTop());
    Point rightBottom = ProjectionToRasterCoord(mappedArea.BottomRight());
    return Rect(leftTop, rightBottom);
}

Area GDALMap::AreaForRect(const Rect& rect)
{
    Point leftTop = RasterToProjectionCoord(rect.LeftTop());
    Point bottomRight = RasterToProjectionCoord(rect.BottomRight());
    return Area(ProjectionReference(), leftTop, bottomRight);
}

std::shared_ptr<Layer> GDALMap::ExecuteQuery(std::string query)
{
    OGRLayer* layer = Dataset()->ExecuteSQL(query.c_str(), nullptr, nullptr);
    if (!layer)
        return nullptr;
    return make_shared<Layer>(layer);
}

GDALDataset* GDALMap::Dataset()
{
    if (!_dataset)
    {
        //_dataset = static_cast<GDALDataset*>(GDALOpen(Filename().c_str(), GA_ReadOnly));
        //TODO: only for ECW driver, make this configurable
        CPLStringList optionList;
        //set maximum memory to 2 gigabytes
        long maximumMemory = 2L * 1024 * 1024 * 1024;
        std::string configString = "ECW_CACHE_MAXMEM=" + to_string(maximumMemory);
        optionList.AddString(configString.c_str());
        _dataset = static_cast<GDALDataset*>(GDALOpenEx(Filename().c_str(), GDAL_OF_RASTER | GA_ReadOnly, nullptr, optionList.List(), nullptr));
    }
    return _dataset;
}

std::tuple<int, int> GDALMap::GetTileSize()
{
    int width, height = 0;
    GDALRasterBand* band = Dataset()->GetRasterBand(1);
    band->GetBlockSize(&width, & height);
    return make_tuple(width, height);
}

GByte* GDALMap::GetDataForBand(int rasterIndex, int x, int y, int width, int height)
{
    GDALRasterBand* band = Dataset()->GetRasterBand(rasterIndex);
    /*printf( "Type=%s, ColorInterp=%s\n",
    GDALGetDataTypeName(band->GetRasterDataType()),
    GDALGetColorInterpretationName(
            band->GetColorInterpretation()) );*/

    size_t rasterBandSize = static_cast<size_t>(width * height);
    GByte* data = static_cast<GByte*>(CPLMalloc(rasterBandSize));

    //TODO Use ReadBlock for efficiency and create tile rects corresponding to the block positions
    //CPLErr error = band->ReadBlock(x, y, data);
//    cout << "Getting tile at x = " << x << ",y = " << y << ", width = " << width << ", height=" << height << endl;
    CPLErr error = band->RasterIO(GDALRWFlag::GF_Read, x, y, width, height, data, width, height, GDALDataType::GDT_Byte,0,0);
    if (error != CPLErr::CE_None)
    {
        cout << "error=" << error << " requesting a tile of " << (width * height) << " bytes" << endl;
        CPLFree(data);
        return nullptr;
    }
    return data;
}

Point GDALMap::RasterToProjectionCoord(Point rasterCoord)
{
    return MapTransform().Transform(rasterCoord);
}

Point GDALMap::ProjectionToRasterCoord(Point projectionCoord)
{
    return MapTransform().ReverseTransform(projectionCoord);
}
