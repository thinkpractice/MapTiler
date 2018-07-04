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
         SaveTile(stepData->Tile(), tileFilename + ".tiff");
		 SaveTile(stepData->MaskTile(), tileFilename + "_mask.tiff");
		 SaveTile(stepData->MaskedTile(), tileFilename +"_masked.tiff");
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
