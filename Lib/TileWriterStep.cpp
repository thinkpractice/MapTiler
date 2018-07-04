#include "TileWriterStep.h"
#include "TileWriter.h"
#include <iostream>
#include "Utils.h"

using namespace std;

TileWriterStep::TileWriterStep(string tileDirectory)
                    :   ProcessingStep(PreProcessing),
                        _tileDirectory(tileDirectory),
                        _numberOfTilesWritten(0)
{
}

TileWriterStep::~TileWriterStep()
{
}

void TileWriterStep::Run()
{
     while (auto stepData = InQueue()->dequeue())
	 {
		 string tileFilename = _tileDirectory + stepData->UniqueId();
		 for (auto geoTile : stepData->Tiles())
		 {
			 string filename = tileFilename + "_" + geoTile.first + ".tiff";
			 SaveTile(geoTile.second, filename);
		 }
		 
		 for (auto geoTile : stepData->ProcessedTiles())
		 {
			 string filename = tileFilename + "_" + geoTile.first + ".tiff";
			 SaveTile(geoTile.second, filename);
		 }
	 }
}

void TileWriterStep::SaveTile(shared_ptr<GeoTile> tile, string tileFilename)
{
    TileWriter tileWriter(make_shared<GdalWriter>());
    tileWriter.Save(tile, tileFilename);

    _numberOfTilesWritten++;
    if (_numberOfTilesWritten % 100 == 0)
        cout << "Number of tiles written: " << to_string(_numberOfTilesWritten) << endl;
}
