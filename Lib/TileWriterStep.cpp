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

void TileWriterStep::ProcessTile(GeoTile* tile, Rect tileRect)
{
    TileWriter tileWriter;
    string tileFilename = _tileDirectory + tile->UniqueId() + ".png";
    tileWriter.Save(tile, tileFilename);
}
