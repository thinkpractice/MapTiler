#include <QApplication>
#include <iostream>
#include <iomanip>
#include <regex>
#include <vector>
#include <string>
#include <chrono>
#include <atomic>
#include "Lib/GLWindow.h"
#include "Lib/Menu.h"
#include "Lib/GeoMapProvider.h"
#include "Lib/Area.h"
#include "Lib/SpatialReference.h"
#include "Lib/Point.h"
#include "Lib/TileWriter.h"
#include "Lib/AreaLookup.h"
#include "Lib/ThreadPool.h"
#include "Lib/SafeQueue.h"
#include "Lib/TileProcessor.h"
#include "Lib/Utils.h"

using namespace std;

void DownloadTilesForArea(GeoMap* chosenMap, const Area& area, int tileWidth, int tileHeight, string tileDirectory, string polygonFilename)
{
    Utils.TimeIt([&]
    {
      TileProcessor processor(chosenMap, area, tileWidth, tileHeight);
      processor.StartProcessing(tileDirectory, polygonFilename);
    });
}

int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("MapTiler");
    QCoreApplication::setApplicationVersion("0.1");

    QCommandLineParser parser;
    parser.setApplicationDescription("MapTiler downloads tiles of a given size from a geo raster webservice (WMS/WTMS) and masks them with a polygon layer.");
    parser.addHelpOption();

    parser.addPositionalArgument("rasterurl", QCoreApplication::translate("main", "Url to raster webservice (WMS/WMTS) with the aerial image."));
    parser.addPositionalArgument("vectorurl", QCoreApplication::translate("main", "Url to the vector webservice (WFS) with the polygons."));
    parser.addOptions({
        {"address", QCoreApplication::translate("main", "The address/city name/region for which the tiles should be downloaded.")},
        {{"t", "target-directory"},
            QCoreApplication::translate("main", "Copy all the tiles into <directory>."),
            QCoreApplication::translate("main", "directory")},
        },
        {{"w","tilewidth"}, QCoreApplication::translate("main","The width of the tiles to be written to disk")},
        {{"h","tileheight"}, QCoreApplication::translate("main","The height of the tiles to be written to disk")}
    });
    string filename = u8"WMTS:https://geodata.nationaalgeoregister.nl/luchtfoto/rgb/wmts/1.0.0/WMTSCapabilities.xml";
    string polygonFilename = "WFS:https://geodata.nationaalgeoregister.nl/bag/wfs?SERVICE=wfs";
    string address = "Heerlen";
    string tileDirectory = "/media/tim/Data/Work/CBS/Tiles/";
    //string tileDirectory = "/home/tjadejong/Documents/CBS/ZonnePanelen/Tiles/";

    int tileWidth = 256;
    int tileHeight = 256;

    if (argc > 1)
    {
        tileDirectory = string(argv[1]);
        cout << "output directory: " << tileDirectory << endl;
    }

    GeoMapProvider mapProvider(filename);
    if (mapProvider.Maps().size() == 0)
    {
        cout << "No maps at url/in file" << endl;\
        return -1;
    }

    GeoMap* chosenMap = mapProvider.Maps()[1];
    if (mapProvider.Maps().size() > 1)
    {
        cout << "Multiple Maps found at url, please choose the one you would like to use:" << endl;
        chosenMap = Menu<GeoMap*>::ShowMenu(mapProvider.Maps(), [&](int i, GeoMap* dataset){
                string menuItem = to_string(i) + ") title=" + dataset->Title() + ", url=" + dataset->Filename() + "\n";
                return menuItem;
            });
    }

    cout << "===GeoTransform===" << endl;
    double geoTransform[6];
    chosenMap->MapTransform().GetTransformMatrix(geoTransform);
    for (int i = 0; i < 6; i++)
    {
        cout << geoTransform[i] << endl;
    }

    cout << "===GeoTransform===" << endl;

    cout << "layer count=" << chosenMap->LayerCount() << endl;
    cout << "raster count=" << chosenMap->RasterCount() << endl;
    cout << "raster X size=" << chosenMap->WidthInPixels() << endl;
    cout << "raster Y size=" << chosenMap->HeightInPixels() << endl;

    Area mapArea = chosenMap->GetMapArea();
    cout << "MapArea(" << mapArea.LeftTop().X << "," << mapArea.LeftTop().Y << "," << mapArea.BottomRight().X << "," << mapArea.BottomRight().Y << ")" << endl;

    AreaLookup areaLookup;
    areaLookup.AddListener([&](vector<Area> areas){

                if (areas.size() > 0)
                {
                    Area chosenArea = Menu<Area>::ShowMenu(areas, [](int i, Area area)
                            {
                                return to_string(i) + ") " + area.Description();
                            });
                    DownloadTilesForArea(chosenMap, chosenArea, tileWidth, tileHeight, tileDirectory, polygonFilename);
                }
            });
    for (auto& serviceProvider : areaLookup.ServiceProviders())
    {
        std::cout << serviceProvider << std::endl;
    }
    areaLookup.GetAreaForAddress(address);

    return app.exec();
}
