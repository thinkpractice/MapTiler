#ifndef TILEWRITERSTEP_H
#define TILEWRITERSTEP_H

#include <string>
#include <memory>
#include "ProcessingStep.h"
#include "GeoTile.h"
#include "DatabaseWrapper.h"

class TileWriterStep : public ProcessingStep
{
    public:
        TileWriterStep(std::string tileDirectory, std::string persistenceUrl, std::string driverName, std::string epsgFormat, std::string fileExtension);
        virtual ~TileWriterStep();

        void Run();
        void SaveTile(std::shared_ptr<GeoTile> tile, string tileFilename);

        std::string DriverName() const;
        std::string EpsgFormat() const;
        std::string FileExtension() const;

private:
        void WriteTile(std::shared_ptr<DatabaseWrapper> databasePersistence, std::shared_ptr<StepData> stepData, std::pair<std::string, StepData::TileData> geoTile);

private:
        std::string _tileDirectory;
        std::string _persistenceUrl;
        std::string _driverName;
        std::string _epsgFormat;
        std::string _fileExtension;
        int _numberOfTilesWritten;
};

#endif /* TILEWRITERSTEP_H */
