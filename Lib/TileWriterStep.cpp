#include "TileWriterStep.h"
#include "TileWriter.h"

TileWriterStep::TileWriterStep(string tileDirectory)
                    :   ProcessingStep(PreProcessing),
                        _tileDirectory(tileDirectory)
{
}

TileWriterStep::~TileWriterStep()
{
}

void TileWriterStep::Run()
{
     GeoTile* tileToProcess = InQueue()->dequeue();
     SaveTile(tileToProcess);
}

void TileWriterStep::SaveTile(GeoTile* tile)
{
    TileWriter tileWriter;
    string tileFilename = _tileDirectory + tile->UniqueId() + ".png";
    tileWriter.Save(tile, tileFilename);
}
