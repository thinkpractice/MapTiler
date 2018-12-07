#include <QApplication>
#include <QCoreApplication>
#include <QtCore>
#include <iostream>
#include <vector>
#include <string>
#include "Lib/Area.h"
#include "Lib/AreaLookup.h"
#include "Lib/Utils.h"
#include "Lib/Settings.h"
#include "Lib/StepFactory.h"
#include "Lib/CommandLineParser.h"

class Task : public QObject
{
    Q_OBJECT
public:
    Task(QObject *parent, const Settings& settings)
        : QObject(parent), _settings(settings)
    {
    }

protected:
    void DownloadTilesForArea(const Settings& settings)
    {
        Utils::TimeIt([&]
        {
            StepFactory stepFactory;
            auto processingPipeline = stepFactory.PipelineFor(settings);
            processingPipeline->StartProcessing();
        });
        std::cout << "Finished" << std::endl;
    }

public slots:
    void run()
    {
        std::cout << "Target directory: " << _settings.OutputDirectory() << std::endl;
        std::cout << "Address: " << _settings.Address() << std::endl;
        std::cout << "Tile width: " << _settings.TileWidth() << std::endl;
        std::cout << "Tile height: " << _settings.TileHeight() << std::endl;

        AreaLookup areaLookup;
        areaLookup.AddListener([&](std::vector<Area> areas){

            if (areas.size() > 0)
            {
                Area chosenArea = areas.front();
                if (areas.size() > 1)
                {
                    chosenArea = areas[_settings.AddressOption()];
                }
                _settings.SetChosenArea(chosenArea);
                std::cout << "area=" << chosenArea.LeftTop() << "," << chosenArea.BottomRight() << std::endl;
                DownloadTilesForArea(_settings);
            }
            });

        if (_settings.Address().empty())
            DownloadTilesForArea(_settings);
        else
            areaLookup.GetAreaForAddress(_settings.Address());
        emit finished();
    }

signals:
    void finished();

private:
    Settings _settings;
};

#include "MapTiler.moc"

int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("MapTiler");
    QCoreApplication::setApplicationVersion("0.3");

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
	
    Task *task = new Task(&app, settings);
    QObject::connect(task, SIGNAL(finished()), &app, SLOT(quit()));
    QTimer::singleShot(0, task, SLOT(run()));

    return app.exec();
}
