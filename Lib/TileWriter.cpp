#include "TileWriter.h"
#include <iostream>
#include "Utils.h"
#include "AffineTransform.h"
#include "CoordinateTransformation.h"
#include <png.h>
#include <gdal/cpl_string.h>

GeoTileWriter::GeoTileWriter()
{
}

GeoTileWriter::~GeoTileWriter()
{
}

TileWriter::TileWriter(std::unique_ptr<GeoTileWriter> writer)
                :   _writer(std::move(writer))
{
}

void TileWriter::Save(shared_ptr<GeoTile> tile, string filename)
{
    _writer->Save(tile, filename);
}

string TileWriter::FileExtension() const
{
    return _writer->FileExtension();
}

GdalWriter::GdalWriter(std::string epsgFormat, std::string driverName, std::string fileExtension, std::vector<std::string> driverOptions)
                :   GdalWriter(SpatialReference::FromEPSG(epsgFormat), driverName, fileExtension, driverOptions)
{
}

GdalWriter::GdalWriter(const SpatialReference& targetProjection, std::string driverName, std::string fileExtension, std::vector<std::string> driverOptions)
                :   _driver(nullptr),
                    _targetProjection(targetProjection),
                    _driverName(driverName),
                    _fileExtension(fileExtension),
                    _driverOptions(driverOptions)
{
}

GdalWriter::~GdalWriter()
{
}

bool GdalWriter::HandlesFile(string filename)
{
    return Utils::GetFileExtension(filename) == FileExtension();
}

void GdalWriter::Save(shared_ptr<GeoTile> tile, string filename)
{
    auto mapForTile = MapFor(tile, filename);
    if (!mapForTile)
        return;
    mapForTile->SetProjectionReference(_targetProjection);

    //Convert tile area into target projection
    Area targetArea = CoordinateTransformation::MapArea(tile->BoundingArea(), _targetProjection);
    mapForTile->SetMapTransform(AffineTransform::FromAreaAndRect(targetArea, tile->BoundingRect()));

    mapForTile->WriteTile(tile);
}

std::string GdalWriter::DriverName() const
{
    return _driverName;
}

string GdalWriter::FileExtension() const
{
    return _fileExtension;
}

std::vector<string> GdalWriter::DriverOptions() const
{
    return _driverOptions;
}

shared_ptr<GeoMap> GdalWriter::MapFor(shared_ptr<GeoTile> tile, string filename)
{
    GDALDriver* driver = DriverFor(DriverName());
    if (!SupportsCreate(driver))
    {
        cerr << "Driver does not support create" << endl;
        return nullptr;
    }

    CPLStringList optionList;
    for (auto& driverOption : DriverOptions())
    {
        optionList.AddString(driverOption.c_str());
    }

    Rect boundingRect = tile->BoundingRect();
    GDALDataset *dataset = driver->Create(filename.c_str(), boundingRect.IntegerWidth(), boundingRect.IntegerHeight(), tile->NumberOfLayers(), GDT_Byte, optionList.List());

    return make_shared<GDALMap>(filename, dataset);
}

GDALDriver* GdalWriter::DriverFor(string fileFormat)
{
    if (!_driver)
        _driver = GetGDALDriverManager()->GetDriverByName(fileFormat.c_str());
    return _driver;
}

bool GdalWriter::SupportsCreate(GDALDriver* driver)
{
    char **papszMetadata = driver->GetMetadata();
    if(CSLFetchBoolean(papszMetadata, GDAL_DCAP_CREATE, FALSE ))
        return true;
    return false;
}
