#include "CommandLineParser.h"
#include <QCoreApplication>
#include "GeoMapProvider.h"
#include "Utils.h"

CommandLineParser::CommandLineParser()
{
}

CommandLineParser::CommandLineParseResult CommandLineParser::Parse()
{
    _parser.setApplicationDescription("MapTiler downloads tiles of a given size from a geo raster webservice (WMS/WTMS) and masks them with a polygon layer.");
    _parser.addPositionalArgument("rasterurl", QCoreApplication::translate("main", "Url to raster webservice (WMS/WMTS) with the aerial image."));

    const QCommandLineOption _helpOption = _parser.addHelpOption();
    const QCommandLineOption _versionOption = _parser.addVersionOption();

    _parser.addOptions({{"display_datasets", QCoreApplication::translate("main", "Displays the datasets available at <url>, the ends the app."), "url", ""},
        {{"p", "pipeline_settings"}, QCoreApplication::translate("main", "Loads a Json file with a pipeline settings, with the filename <pipeline_settings>"), "pipeline_settings", ""},
        {"address", QCoreApplication::translate("main", "The <location> (address/city name/region) for which the tiles should be downloaded.")},
        {{"a","addressoption"}, QCoreApplication::translate("main", "The location option to choose if the address gives back multiple option (default=first)"), "locationoption", "0"},
        {{"t", "target-directory"},
            QCoreApplication::translate("main", "Copy all the tiles into <directory>."),
            QCoreApplication::translate("main", "directory"), "/media/tim/Data/Work/CBS/Tiles/"},
        {{"c","tilewidth"}, QCoreApplication::translate("main","The <width> (number of columns) of the tiles to be written to disk")},
        {{"r","tileheight"}, QCoreApplication::translate("main","The <height> (number of rows) of the tiles to be written to disk")}
    });

    if (!_parser.parse(QCoreApplication::arguments()))
    {
        cerr << _parser.errorText().toStdString() << endl;
        cerr << endl << endl;
        cerr << qPrintable(_parser.helpText()) << endl;
        return CommandLineError;
    }

    if (_parser.isSet(_versionOption))
    {
        printf("%s %s\n", qPrintable(QCoreApplication::applicationName()),
                   qPrintable(QCoreApplication::applicationVersion()));
        return CommandLineVersionRequested;
    }

    if (_parser.isSet(_helpOption))
    {
        _parser.showHelp();
        return CommandLineHelpRequested;
    }

    std::string displayUrl = GetStringValue("display_datasets");
    if (!displayUrl.empty())
    {
        DisplayUrl(displayUrl);
        return CommandLineDisplayUrl;
    }

    //TODO add default pipeline
    std::string pipelineSettingsFilename = GetStringValue("pipeline_settings");
    if (!Utils::FileExists(pipelineSettingsFilename))
    {
        std::cerr << "PipeLine File: " << pipelineSettingsFilename << " does not exist." << std::endl;
        return CommandLineError;
    }
    _settings = Settings::Open(pipelineSettingsFilename);

    _settings.SetAddress(GetStringValue("address", _settings.Address()));
    _settings.SetAddressOption(GetIntValue("addressoption"));
    _settings.SetOutputDirectory(GetStringValue("t"));

    _settings.SetTileWidth(GetIntValue("c", _settings.TileWidth()));
    _settings.SetTileHeight(GetIntValue("r", _settings.TileHeight()));

    const QStringList positionalArguments = _parser.positionalArguments();
    std::string mainRasterUrl = _settings.MainRasterUrl();
    if (!positionalArguments.isEmpty())
    {
        _settings.SetMainRasterUrl(mainRasterUrl);
    }
    return CommandLineOk;
}

std::string CommandLineParser::ErrorMessage() const
{
    return _errorMessage;
}

std::string CommandLineParser::GetStringValue(std::string settingsName, std::string defaultValue)
{
   QString value = _parser.value(QString::fromStdString(settingsName));
   if (value.isEmpty())
       return defaultValue;
   return value.toStdString();
}

int CommandLineParser::GetIntValue(std::string settingsName, int defaultValue)
{
   QString value = _parser.value(QString::fromStdString(settingsName));
   if (value.isEmpty())
       return defaultValue;
   return value.toInt();
}

Settings CommandLineParser::GetSettings()
{
    return _settings;
}

void CommandLineParser::DisplayUrl(std::string displayUrl)
{
    GeoMapProvider mapProvider(displayUrl);
    for (auto map : mapProvider.Maps())
    {
        cout << "title=" + map->Title() + ", url=" + map->Filename() << endl;
    }
}
