#include "TileWriterStep.h"
#include "TileWriter.h"
#include "Utils.h"
#include <iostream>

TileWriterStep::TileWriterStep(std::string tileDirectory, std::string persistenceUrl, std::string driverName, std::string epsgFormat, std::string fileExtension)
                    :   ProcessingStep(Sink),
                        _tileDirectory(tileDirectory),
                        _persistenceUrl(persistenceUrl),
                        _driverName(driverName),
                        _epsgFormat(epsgFormat),
                        _fileExtension(fileExtension),
                        _numberOfTilesWritten(0)
{
}

TileWriterStep::~TileWriterStep()
{
}

void TileWriterStep::WriteTile(std::shared_ptr<DatabaseWrapper> databasePersistence, std::shared_ptr<StepData> stepData, std::pair<std::string, StepData::TileData> geoTile)
{
    std::string tileFilename = _tileDirectory + stepData->UniqueId();
    std::string filename = tileFilename + "_" + geoTile.first;
    SaveTile(geoTile.second.tile, filename);
    if (databasePersistence)
        databasePersistence->SaveTileFile(stepData->TileId(), filename, geoTile.first, geoTile.second.year);
}

void TileWriterStep::Run()
{
     std::shared_ptr<DatabaseWrapper> databasePersistence = DatabaseWrapper::DatabaseWrapperFor(_persistenceUrl);
     while (auto stepData = InQueue()->dequeue())
	 {
		 for (auto geoTile : stepData->Tiles())
		 {
             WriteTile(databasePersistence, stepData, geoTile);
		 }
		 
		 for (auto geoTile : stepData->ProcessedTiles())
		 {
             WriteTile(databasePersistence, stepData, geoTile);
         }
         _numberOfTilesWritten++;
         if (_numberOfTilesWritten % 100 == 0)
             std::cout << "Number of tiles written: " << to_string(_numberOfTilesWritten) << std::endl;
	 }
     DoneProcessing();
}

void TileWriterStep::SaveTile(std::shared_ptr<GeoTile> tile, std::string tileFilename)
{
        TileWriter tileWriter(make_shared<GdalWriter>(EpsgFormat(), DriverName(), FileExtension()));
        tileWriter.Save(tile, tileFilename + "." + tileWriter.FileExtension());
}

std::string TileWriterStep::DriverName() const
{
    return _driverName;
}

std::string TileWriterStep::EpsgFormat() const
{
    return _epsgFormat;
}

std::string TileWriterStep::FileExtension() const
{
    return _fileExtension;
}
