#include <QApplication>
#include <QCommandLineParser>
#include <QCoreApplication>
#include <iostream>
#include <iomanip>
#include <regex>
#include <vector>
#include <map>
#include <string>
#include <chrono>
#include <atomic>
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

using namespace std;

enum CommandLineParseResult
{
    CommandLineOk,
    CommandLineError,
    CommandLineVersionRequested,
    CommandLineHelpRequested
};

CommandLineParseResult ParseCommandLine(QCommandLineParser &parser, MapTilerSettings *settings, string *errorMessage)
{
    //parser.setSingleDashWordOptionMode(QCommandLineParser::ParseAsLongOptions);
    parser.setApplicationDescription("MapTiler downloads tiles of a given size from a geo raster webservice (WMS/WTMS) and masks them with a polygon layer.");
    parser.addPositionalArgument("rasterurl", QCoreApplication::translate("main", "Url to raster webservice (WMS/WMTS) with the aerial image."));
    parser.addPositionalArgument("vectorurl", QCoreApplication::translate("main", "Url to the vector webservice (WFS) with the polygons."));

    QCommandLineOption useDefaultUrls({"d", "defaulturls"}, QCoreApplication::translate("main", "Uses default urls for rasterurl and vectorurl"));
    parser.addOption(useDefaultUrls);

    const QCommandLineOption helpOption = parser.addHelpOption();
    const QCommandLineOption versionOption = parser.addVersionOption();

    parser.addOptions({
		{"display_datasets", QCoreApplication::translate("main", "Displays the datasets available at <url>, the ends the app."), "url", ""},
        {"address", QCoreApplication::translate("main", "The <location> (address/city name/region) for which the tiles should be downloaded."), "location", "Heerlen"},
        {{"a","addressoption"}, QCoreApplication::translate("main", "The location option to choose if the address gives back multiple option (default=first)"), "locationoption", "0"},              
        {{"t", "target-directory"},
            QCoreApplication::translate("main", "Copy all the tiles into <directory>."),
            QCoreApplication::translate("main", "directory"), "/media/tim/Data/Work/CBS/Tiles/"},
        {{"c","tilewidth"}, QCoreApplication::translate("main","The <width> (number of columns) of the tiles to be written to disk"), "width", "256"},
        {{"r","tileheight"}, QCoreApplication::translate("main","The <height> (number of rows) of the tiles to be written to disk"), "height", "256"}
    });

    if (!parser.parse(QCoreApplication::arguments()))
    {
        *errorMessage = parser.errorText().toStdString();
        return CommandLineError;
    }

    if (parser.isSet(versionOption))
        return CommandLineVersionRequested;

    if (parser.isSet(helpOption))
        return CommandLineHelpRequested;

    QString displayUrl = parser.value("display_datasets");
    if (!displayUrl.isEmpty())
	{
        settings->displayUrl = displayUrl.toStdString();
		return CommandLineOk;
	}
	
    QString address = parser.value("address");
    if (!address.isEmpty())
        settings->address = address.toStdString();

    QString addressOption = parser.value("addressoption");
    if (!addressOption.isEmpty())
        settings->addressOption = addressOption.toInt();
    
    QString targetDirectory = parser.value("t");
    if (!targetDirectory.isEmpty())
        settings->targetDirectory = targetDirectory.toStdString();

    QString tileWidth = parser.value("c");
    if (!tileWidth.isEmpty())
        settings->tileWidth = tileWidth.toInt();

    QString tileHeight = parser.value("r");
    if (!tileHeight.isEmpty())
        settings->tileHeight = tileHeight.toInt();

    if (parser.isSet(useDefaultUrls))
        return CommandLineOk;

    const QStringList positionalArguments = parser.positionalArguments();
    if (positionalArguments.isEmpty())
    {
        *errorMessage = "Raster url and vector url arguments missing.";
        return CommandLineError;
    }
    if (positionalArguments.size() <= 1)
    {
        *errorMessage = "An url argument is missing.";
        return CommandLineError;
    }
    settings->mainRasterFilename = positionalArguments[0].toStdString();
    settings->metadataFilenames["polygons"] = {positionalArguments[1].toStdString(), 1};

    return CommandLineOk;
}

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

void DownloadTilesForArea(const MapTilerSettings& settings, const Area& area)
{
    auto mainRasterMap = GetMapForUrl(settings.mainRasterFilename);

    Utils::TimeIt([&]
    {
        TileProcessor processor(mainRasterMap, area, settings);
		for (auto mapPair : settings.mapFilenames)
		{
			auto map = GetMapForUrl(mapPair.second);
			processor.AddRasterMap(mapPair.first, map);
		}
        processor.StartProcessing();
    });
    cout << "Finished" << endl;
}

int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("MapTiler");
    QCoreApplication::setApplicationVersion("0.1");

    QCommandLineParser parser;
    MapTilerSettings settings;
    string errorMessage;
    switch (ParseCommandLine(parser, &settings, &errorMessage))
    {
        case CommandLineOk:
            break;
        case CommandLineError:
            cerr << errorMessage << endl;
            fputs("\n\n", stderr);
            fputs(qPrintable(parser.helpText()), stderr);
            return 1;
        case CommandLineVersionRequested:
            printf("%s %s\n", qPrintable(QCoreApplication::applicationName()),
                   qPrintable(QCoreApplication::applicationVersion()));
            return 0;
        case CommandLineHelpRequested:
            parser.showHelp();
            Q_UNREACHABLE();
    };

	if (settings.displayUrl != "")
	{
		GeoMapProvider mapProvider(settings.displayUrl);
		for (auto map : mapProvider.Maps())
		{
			cout << "title=" + map->Title() + ", url=" + map->Filename() << endl;	
		}
		
		QCoreApplication::exit(0);
		return 0;
	}
	
    for (auto metadataPair : settings.metadataFilenames)
        cout << "Metadata: " << metadataPair.first << " url: " << metadataPair.second.metadataFilename << " layerIndex: " << metadataPair.second.layerIndex << endl;
    cout << "Target directory: " << settings.targetDirectory << endl;
    cout << "Address: " << settings.address << endl;
    cout << "Tile width: " << settings.tileWidth << endl;
    cout << "Tile height: " << settings.tileHeight << endl;

    AreaLookup areaLookup;
    areaLookup.AddListener([&](vector<Area> areas){

                if (areas.size() > 0)
                {
                    Area chosenArea = areas.front();
                    if (areas.size() > 1)
                    {
                            chosenArea = areas[settings.addressOption];
                            
                            /*chosenArea = Menu<Area>::ShowMenu(areas, [](int i, Area area)
                            {
                                return to_string(i) + ") " + area.Description();
                            });*/
                    }
                    DownloadTilesForArea(settings, chosenArea);
					QCoreApplication::exit(0);
                }
            });
    areaLookup.GetAreaForAddress(settings.address);

    return app.exec();
}
