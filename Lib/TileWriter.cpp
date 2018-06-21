#include "TileWriter.h"
#include <iostream>
#include "Utils.h"
#include "AffineTransform.h"
#include "CoordinateTransformation.h"
#include <png.h>

TileWriter::TileWriter(shared_ptr<GeoTileWriter> writer)
                :   _writer(writer)
{
}

void TileWriter::Save(shared_ptr<GeoTile> tile, string filename)
{
    _writer->Save(tile, filename);
}

PngWriter::PngWriter()
            :   GeoTileWriter()
{
}

bool PngWriter::HandlesFile(string filename)
{
    return Utils::GetFileExtension(filename) == "png";
}

void PngWriter::Save(shared_ptr<GeoTile> tile, string filename)
{
    //cout << "Writing tile to file: " << filename << endl;

    int code = 0;
    FILE *fp = NULL;
    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;
    png_bytep row = NULL;

    int width = tile->BoundingRect().Width();
    int height = tile->BoundingRect().Height();

    unsigned char* rgba = tile->Data();

    fp = fopen(filename.c_str(), "wb");
    if (fp == NULL)
    {
      fprintf(stderr, "Could not open file %s for writing\n", filename.c_str());
      code = 1;
      goto finalise;
    }

    // Initialize write structure
    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png_ptr == NULL)
    {
      fprintf(stderr, "Could not allocate write struct\n");
      code = 1;
      goto finalise;
    }

    // Initialize info structure
    info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL)
    {
      fprintf(stderr, "Could not allocate info struct\n");
      code = 1;
      goto finalise;
    }

    // Setup Exception handling
    if (setjmp(png_jmpbuf(png_ptr))) {
      fprintf(stderr, "Error during png creation\n");
      code = 1;
      goto finalise;
    }

    png_init_io(png_ptr, fp);

    // Write header (8 bit colour depth)
    png_set_IHDR(png_ptr, info_ptr,
            width,
            height,
         8, PNG_COLOR_TYPE_RGBA, PNG_INTERLACE_NONE,
         PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    // Set title
    /*if (title != NULL) {
      png_text title_text;
      title_text.compression = PNG_TEXT_COMPRESSION_NONE;
      title_text.key = "Title";
      title_text.text = title;
      png_set_text(png_ptr, info_ptr, &title_text, 1);
    }*/

    png_write_info(png_ptr, info_ptr);

    // Allocate memory for one row (3 bytes per pixel - RGB)
    row = (png_bytep) malloc(4 * width * sizeof(png_byte));


    // Write image data
    int x, y;
    for (y=0 ; y<height ; y++) {
        for (x=0 ; x<width ; x++) {
            //  setRGB(&(row[x*3]), buffer[y*width + x]);
            row[x*4] = rgba[(y*width +x)*4];
            row[x*4+1] = rgba[(y*width +x)*4+1];
            row[x*4+2] = rgba[(y*width +x)*4+2];
            row[x*4+3] = rgba[(y*width +x)*4+3];
        }
        png_write_row(png_ptr, row);
    }

    // End write
    png_write_end(png_ptr, NULL);

finalise:
    if (fp != NULL)
        fclose(fp);
    if (info_ptr != NULL)
        png_free_data(png_ptr, info_ptr, PNG_FREE_ALL, -1);
    if (png_ptr != NULL)
        png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
    if (row != NULL)
        free(row);

    //return code;
}

GdalWriter::GdalWriter()
                :   GdalWriter(SpatialReference::FromEPSG("EPSG:4326"))
{
}

GdalWriter::GdalWriter(const SpatialReference& targetProjection)
                :   _targetProjection(targetProjection)
{
}

GdalWriter::~GdalWriter()
{
}

bool GdalWriter::HandlesFile(string filename)
{
    return Utils::GetFileExtension(filename) == "tiff";
}

void GdalWriter::Save(shared_ptr<GeoTile> tile, string filename)
{
    auto mapForTile = MapFor(tile, filename);
    mapForTile->SetProjectionReference(_targetProjection);

    //Convert tile area into target projection
    Area targetArea = CoordinateTransformation::MapArea(tile->BoundingArea(), _targetProjection);
    mapForTile->SetMapTransform(AffineTransform::FromAreaAndRect(targetArea, tile->BoundingRect()));

    mapForTile->WriteTile(tile);
}

shared_ptr<GeoMap> GdalWriter::MapFor(shared_ptr<GeoTile> tile, string filename)
{
    GDALDriver* driver = DriverFor("GTiff");
    if (!SupportsCreate(driver))
    {
        cerr << "Driver does not support create" << endl;
        return;
    }

    char **papszOptions = nullptr;
    Rect boundingRect = tile->BoundingRect();
    GDALDataset *dataset = driver->Create(filename.c_str(), boundingRect.Width(), boundingRect.Height(), tile->NumberOfLayers(), GDT_Byte, papszOptions);
    return make_shared<GDALMap>(filename, dataset);
}

GDALDriver* GdalWriter::DriverFor(string fileFormat)
{
    return GetGDALDriverManager()->GetDriverByName(fileFormat.c_str());
}

bool GdalWriter::SupportsCreate(GDALDriver* driver)
{
    char **papszMetadata = poDriver->GetMetadata();
    if(CSLFetchBoolean(papszMetadata, GDAL_DCAP_CREATE, FALSE ))
        return true;
    return false;
}
