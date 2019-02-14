#include "TileWriterStep.h"
#include "Utils.h"
#include <algorithm>
#include <iostream>

TileWriterStep::TileWriterStep(std::string tileDirectory, std::string persistenceUrl, std::string driverName, int epsgFormat, std::string fileExtension, std::vector<std::string> driverOptions)
                    :   ProcessingStep(Sink),
                        _tileDirectory(tileDirectory),
                        _persistenceUrl(persistenceUrl),
                        _driverName(driverName),
                        _epsgFormat(epsgFormat),
                        _fileExtension(fileExtension),
                        _driverOptions(driverOptions),
                        _tileWriter(make_unique<GdalWriter>(EpsgFormat(), DriverName(), FileExtension(), DriverOptions())),
                        _numberOfTilesWritten(0)
{
}

TileWriterStep::~TileWriterStep()
{
}

void TileWriterStep::WriteTile(std::shared_ptr<DatabaseWrapper> databasePersistence, std::string uniqueId, long long tileId, std::string layerName, int year, std::shared_ptr<GeoTile> geoTile)
{
    std::string tileFilename = _tileDirectory + uniqueId;
    std::string filename = tileFilename + "_" + layerName;
    SaveTile(geoTile, filename);
    if (databasePersistence)
        databasePersistence->SaveTileFile(tileId, filename, layerName, year);
}

void TileWriterStep::Run()
{
     std::shared_ptr<DatabaseWrapper> databasePersistence = DatabaseWrapper::DatabaseWrapperFor(_persistenceUrl);
     while (auto stepData = InQueue()->dequeue())
	 {
         for (auto geoTile : stepData->Tiles())
		 {
             WriteTile(databasePersistence, stepData->UniqueId(), stepData->TileId(), geoTile.first, geoTile.second.year, geoTile.second.tile);
         }
		 
		 for (auto geoTile : stepData->ProcessedTiles())
		 {
              WriteTile(databasePersistence, stepData->UniqueId(), stepData->TileId(), geoTile.first, geoTile.second.year, geoTile.second.tile);
         }
         _numberOfTilesWritten++;
         if (_numberOfTilesWritten % 100 == 0)
             std::cout << "Number of tiles written: " << to_string(_numberOfTilesWritten) << std::endl;
	 }
     DoneProcessing();
}

void TileWriterStep::SaveTile(std::shared_ptr<GeoTile> tile, std::string tileFilename)
{
    _tileWriter.Save(tile, tileFilename + "." + _tileWriter.FileExtension());
}

std::string TileWriterStep::DriverName() const
{
    return _driverName;
}

int TileWriterStep::EpsgFormat() const
{
    return _epsgFormat;
}

std::string TileWriterStep::FileExtension() const
{
    return _fileExtension;
}

std::vector<string> TileWriterStep::DriverOptions() const
{
    return _driverOptions;
}
