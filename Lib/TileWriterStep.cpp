#include "TileWriterStep.h"
#include "TileWriter.h"
#include "DatabaseWrapper.h"
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

void TileWriterStep::Run()
{
     std::shared_ptr<DatabaseWrapper> databasePersistence = DatabaseWrapper::DatabaseWrapperFor(_persistenceUrl);
     while (auto stepData = InQueue()->dequeue())
	 {
         std::string tileFilename = _tileDirectory + stepData->UniqueId();
		 for (auto geoTile : stepData->Tiles())
		 {
             std::string filename = tileFilename + "_" + geoTile.first;
             SaveTile(geoTile.second.tile, filename);
             if (databasePersistence)
                 databasePersistence->SaveTileFile(stepData->TileId(), filename, geoTile.first, geoTile.second.year);
		 }
		 
		 for (auto geoTile : stepData->ProcessedTiles())
		 {
             std::string filename = tileFilename + "_" + geoTile.first;
             SaveTile(geoTile.second.tile, filename);
             if (databasePersistence)
                 databasePersistence->SaveTileFile(stepData->TileId(), filename, geoTile.first, geoTile.second.year);
		 }
         _numberOfTilesWritten++;
         if (_numberOfTilesWritten % 100 == 0)
             std::cout << "Number of tiles written: " << to_string(_numberOfTilesWritten) << std::endl;
	 }
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
