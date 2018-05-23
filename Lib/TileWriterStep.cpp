#include "TileWriterStep.h"
#include "TileWriter.h"
#include <iostream>
#include "Utils.h"

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
     while (GeoTile* tileToProcess = InQueue()->dequeue())
         SaveTile(tileToProcess);
}

void TileWriterStep::SaveTile(GeoTile* tile)
{
    Utils::TimeIt([&]
            {
    TileWriter tileWriter;
    string tileFilename = _tileDirectory + tile->UniqueId() + ".png";
    tileWriter.Save(tile, tileFilename);
    
    _numberOfTilesWritten++;
    cout << "Number of tiles written: " << to_string(_numberOfTilesWritten) << endl;
    });
}
