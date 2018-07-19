#include <QApplication>
#include <QCoreApplication>
#include <iostream>
#include <iomanip>
#include <regex>
#include <vector>
#include <map>
#include <string>
#include <chrono>
#include <atomic>
#include <fstream>
#include "Lib/MapTilerSettings.h"
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
#include "Lib/Settings.h"
#include "Lib/StepFactory.h"
#include "Lib/CommandLineParser.h"

using namespace std;

shared_ptr<GeoMap> GetMapForUrl(string url)
{
    GeoMapProvider mapProvider(url);
    if (mapProvider.Maps().size() == 0)
    {
        cout << "No maps at url/in file" << endl;\
        return nullptr;
    }

    auto chosenMap = mapProvider.Maps().front();
    if (mapProvider.Maps().size() > 1)
    {
        cout << "Multiple Maps found at url, please choose the one you would like to use:" << endl;
        chosenMap = Menu<shared_ptr<GeoMap>>::ShowMenu(mapProvider.Maps(), [&](int i, shared_ptr<GeoMap> dataset){
                string menuItem = to_string(i) + ") title=" + dataset->Title() + ", url=" + dataset->Filename() + "\n";
                return menuItem;
            });
    }

    cout << "Starting MapTiler for map at: " << chosenMap->Title() << endl;

    cout << "GeoTransform: (";
    double geoTransform[6];
    chosenMap->MapTransform().GetTransformMatrix(geoTransform);
    for (int i = 0; i < 6; i++)
    {
        cout << geoTransform[i];
        if (i < 5) cout << ",";
    }
    cout << ")" << endl;

    cout << "Layer count: " << chosenMap->LayerCount() << endl;
    cout << "Raster count: " << chosenMap->RasterCount() << endl;
    cout << "Raster X size: " << chosenMap->WidthInPixels() << endl;
    cout << "Raster Y size: " << chosenMap->HeightInPixels() << endl;
	
	Area mapArea = chosenMap->GetMapArea();
	cout << "MapArea: (" << mapArea.LeftTop().X << "," << mapArea.LeftTop().Y << "," << mapArea.BottomRight().X << "," << mapArea.BottomRight().Y << ")" << endl;
	return chosenMap;
}

void DownloadTilesForArea(const Settings& settings)
{
    Utils::TimeIt([&]
    {
        StepFactory stepFactory;
        auto processingPipeline = stepFactory.PipelineFor(settings);
        processingPipeline->StartProcessing();
    });
    cout << "Finished" << endl;
}

int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("MapTiler");
    QCoreApplication::setApplicationVersion("0.1");

    CommandLineParser parser;
    switch (parser.Parse())
    {
        case CommandLineParser::CommandLineOk:
            break;
        case CommandLineParser::CommandLineError:
            return 1;
        default:
            return 0;
    };

    Settings settings = parser.GetSettings();
	
    cout << "Target directory: " << settings.OutputDirectory() << endl;
    cout << "Address: " << settings.Address() << endl;
    cout << "Tile width: " << settings.TileWidth() << endl;
    cout << "Tile height: " << settings.TileHeight() << endl;

    AreaLookup areaLookup;
    areaLookup.AddListener([&](vector<Area> areas){

                if (areas.size() > 0)
                {
                    Area chosenArea = areas.front();
                    if (areas.size() > 1)
                    {
                            chosenArea = areas[settings.AddressOption()];
                    }
                    settings.SetChosenArea(chosenArea);
                    DownloadTilesForArea(settings);
					QCoreApplication::exit(0);
                }
            });

    if (settings.Address().empty())
        DownloadTilesForArea(settings);
    else
        areaLookup.GetAreaForAddress(settings.Address());

    return app.exec();
}
