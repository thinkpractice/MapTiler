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

using namespace std;

void WriteTile(GeoTile* tile, string tileDirectory, int currentIndex, int maxIndex)
{
    TileWriter tileWriter;
    double onePercent = maxIndex / 100.0;
    double totalNumberOfBytes = currentIndex * tile->NumberOfBytes();
    if (currentIndex % (int)onePercent == 0)
    {
        //auto t2 = std::chrono::high_resolution_clock::now();
        //auto duration = std::chrono::duration_cast<std::chrono::seconds>(t2-t1).count();
        int imagePercentage = (currentIndex / onePercent) + 1;
        cout << "Wrote " << imagePercentage << "\% out of " << maxIndex << " images(" << currentIndex << " Images, " << fixed << setprecision(2) << (totalNumberOfBytes / 1024) << "KB in " << endl; // duration << " seconds)" << endl;
        
    }
    string tileFilename = tileDirectory + to_string(currentIndex) + ".png" ;
    tileWriter.Save(tile, tileFilename);
    delete tile;
}

void DownloadTilesForArea(GeoMap* chosenMap, const Area& area, string tileDirectory, string polygonFilename)
{
    TileProcessor processor(chosenMap, area);
    processor.StartProcessing(tileDirectory, polygonFilename);
}

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

//string tileDirectory = "/media/tim/Data/Work/CBS/Tiles/tile";
    string tileDirectory = "/home/tjadejong/Documents/CBS/ZonnePanelen/Tiles/";
    if (argc > 1)
    {
        tileDirectory = string(argv[1]);
        cout << "output directory: " << tileDirectory << endl;
    }

    string filename = u8"WMTS:https://geodata.nationaalgeoregister.nl/luchtfoto/rgb/wmts/1.0.0/WMTSCapabilities.xml";
    GeoMapProvider mapProvider(filename);

    GeoMap* chosenMap = Menu<GeoMap*>::ShowMenu(mapProvider.Maps(), [&](int i, GeoMap* dataset){
                string menuItem = to_string(i) + ") title=" + dataset->Title() + ", url=" + dataset->Filename() + "\n";
                return menuItem;
            });

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

    string polygonFilename = "WFS:https://geodata.nationaalgeoregister.nl/bag/wfs?SERVICE=wfs";

    AreaLookup areaLookup;
    areaLookup.AddListener([&](vector<Area> areas){

                if (areas.size() > 0)
                {
                    Area chosenArea = Menu<Area>::ShowMenu(areas, [](int i, Area area)
                            {
                                return to_string(i) + ") " + area.Description();
                            });
                    DownloadTilesForArea(chosenMap, chosenArea, tileDirectory, polygonFilename);
                }
            });
    for (auto& serviceProvider : areaLookup.ServiceProviders())
    {
        std::cout << serviceProvider << std::endl;
    }
    //areaLookup.GetAreaForAddress("Heerlen");
    //areaLookup.GetAreaForAddress("Landgraaf");
    Area area(6.00,50.89,6.01,50.88);
    
    DownloadTilesForArea(chosenMap, area, tileDirectory, polygonFilename);

    return app.exec();
}
