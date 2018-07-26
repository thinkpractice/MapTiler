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
                    cout << "area=" << chosenArea.LeftTop() << "," << chosenArea.BottomRight() << endl;
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
