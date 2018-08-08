#include <QApplication>
#include <QCoreApplication>
#include <iostream>
#include <vector>
#include <string>
#include "Lib/Area.h"
#include "Lib/AreaLookup.h"
#include "Lib/Utils.h"
#include "Lib/Settings.h"
#include "Lib/StepFactory.h"
#include "Lib/CommandLineParser.h"

void DownloadTilesForArea(const Settings& settings)
{
    Utils::TimeIt([&]
    {
        StepFactory stepFactory;
        auto processingPipeline = stepFactory.PipelineFor(settings);
        processingPipeline->StartProcessing();
    });
    std::cout << "Finished" << std::endl;
    QCoreApplication::exit(0);
}

int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("MapTiler");
    QCoreApplication::setApplicationVersion("0.2");

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
	
    std::cout << "Target directory: " << settings.OutputDirectory() << std::endl;
    std::cout << "Address: " << settings.Address() << std::endl;
    std::cout << "Tile width: " << settings.TileWidth() << std::endl;
    std::cout << "Tile height: " << settings.TileHeight() << std::endl;

    AreaLookup areaLookup;
    areaLookup.AddListener([&](std::vector<Area> areas){

                if (areas.size() > 0)
                {
                    Area chosenArea = areas.front();
                    if (areas.size() > 1)
                    {
                            chosenArea = areas[settings.AddressOption()];
                    }
                    settings.SetChosenArea(chosenArea);
                    std::cout << "area=" << chosenArea.LeftTop() << "," << chosenArea.BottomRight() << std::endl;
                    DownloadTilesForArea(settings);
                }
            });

    if (settings.Address().empty())
        DownloadTilesForArea(settings);
    else
        areaLookup.GetAreaForAddress(settings.Address());

    return app.exec();
}
