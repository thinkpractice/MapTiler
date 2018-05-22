#include "TileProcessor.h"
#include "TileProducerStep.h"
#include "TileWriterStep.h"

TileProcessor::TileProcessor(GeoMap* mainRasterMap, int tileWidth, int tileHeight)
                :   TileProcessor(mainRasterMap, mainRasterMap->GetMapArea(), tileWidth, tileHeight)
{
}

TileProcessor::TileProcessor(GeoMap* mainRasterMap, const Area& areaToProcess, int tileWidth, int tileHeight)
                :   _mainRasterMap(mainRasterMap),
                    _tileGrid(_mainRasterMap->RectForArea(areaToProcess), areaToProcess, tileWidth, tileHeight)
{
}

void TileProcessor::StartProcessing(string outputDirectory)
{
    _pipeline.AddProcessingStep(new TileProducerStep(_mainRasterMap, _tileGrid));
    _pipeline.AddProcessingStep(new TileWriterStep(outputDirectory));
    _pipeline.StartProcessing();
}
