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
         SaveTile(stepData->Tile());
		 SaveTile(stepData->MaskTile());
		 SaveTile(stepData->MaskedTile());
	 }
}

void TileWriterStep::SaveTile(shared_ptr<GeoTile> tile)
{
    TileWriter tileWriter(make_shared<GdalWriter>());
    string tileFilename = _tileDirectory + tile->UniqueId() + ".tiff";
    tileWriter.Save(tile, tileFilename);

    _numberOfTilesWritten++;
    if (_numberOfTilesWritten % 100 == 0)
        cout << "Number of tiles written: " << to_string(_numberOfTilesWritten) << endl;
}
