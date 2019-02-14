#ifndef TILEWRITERSTEP_H
#define TILEWRITERSTEP_H

#include <string>
#include <memory>
#include <vector>
#include "ProcessingStep.h"
#include "GeoTile.h"
#include "DatabaseWrapper.h"
#include "TileWriter.h"

class TileWriterStep : public ProcessingStep
{
    public:
        TileWriterStep(std::string tileDirectory, std::string persistenceUrl,
                       std::string driverName, int epsgFormat,
                       std::string fileExtension, std::vector<std::string> driverOptions);
        virtual ~TileWriterStep();

        void Run();
        void SaveTile(std::shared_ptr<GeoTile> tile, string tileFilename);

        std::string DriverName() const;
        int EpsgFormat() const;
        std::string FileExtension() const;
        std::vector<std::string> DriverOptions() const;

private:
        void WriteTile(std::shared_ptr<DatabaseWrapper> databasePersistence, std::string uniqueId, long long tileId, std::string layerName, int year, std::shared_ptr<GeoTile> geoTile);

private:
        std::string _tileDirectory;
        std::string _persistenceUrl;
        std::string _driverName;
        int _epsgFormat;
        std::string _fileExtension;
        std::vector<std::string> _driverOptions;
        TileWriter _tileWriter;
        int _numberOfTilesWritten;
};

#endif /* TILEWRITERSTEP_H */
