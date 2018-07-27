#include "TileWriterStep.h"
#include "TileWriter.h"
#include "DatabaseWrapper.h"
#include <iostream>
#include "Utils.h"

using namespace std;

TileWriterStep::TileWriterStep(string tileDirectory, string persistenceUrl)
                    :   ProcessingStep(Sink),
                        _tileDirectory(tileDirectory),
                        _persistenceUrl(persistenceUrl),
                        _numberOfTilesWritten(0)
{
}

TileWriterStep::~TileWriterStep()
{
}

void TileWriterStep::Run()
{
     shared_ptr<DatabaseWrapper> databasePersistence = DatabaseWrapper::DatabaseWrapperFor(_persistenceUrl);
     while (auto stepData = InQueue()->dequeue())
	 {
		 string tileFilename = _tileDirectory + stepData->UniqueId();
		 for (auto geoTile : stepData->Tiles())
		 {
			 string filename = tileFilename + "_" + geoTile.first + ".tiff";
			 SaveTile(geoTile.second, filename);
             if (databasePersistence)
                 databasePersistence->SaveTileFile(stepData->TileId(), filename, geoTile.first, 0);
		 }
		 
		 for (auto geoTile : stepData->ProcessedTiles())
		 {
			 string filename = tileFilename + "_" + geoTile.first + ".tiff";
			 SaveTile(geoTile.second, filename);
             if (databasePersistence)
                 databasePersistence->SaveTileFile(stepData->TileId(), filename, geoTile.first, 0);
		 }
         _numberOfTilesWritten++;
         if (_numberOfTilesWritten % 100 == 0)
             cout << "Number of tiles written: " << to_string(_numberOfTilesWritten) << endl;
	 }
}

void TileWriterStep::SaveTile(shared_ptr<GeoTile> tile, string tileFilename)
{
    TileWriter tileWriter(make_shared<GdalWriter>());
    tileWriter.Save(tile, tileFilename);
}
