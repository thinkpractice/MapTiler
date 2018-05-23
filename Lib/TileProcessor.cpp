#include "TileProcessor.h"
#include "TileProducerStep.h"
#include "TileWriterStep.h"
#include "TileGpuTransferStep.h"

TileProcessor::TileProcessor(GeoMap* mainRasterMap, int tileWidth, int tileHeight)
                :   TileProcessor(mainRasterMap, mainRasterMap->GetMapArea(), tileWidth, tileHeight)
{
}

TileProcessor::TileProcessor(GeoMap* mainRasterMap, const Area& areaToProcess, int tileWidth, int tileHeight)
                :   _mainRasterMap(mainRasterMap),
                    _tileGrid(_mainRasterMap->RectForArea(areaToProcess), areaToProcess, tileWidth, tileHeight)
{
}

TileProcessor::~TileProcessor()
{
}

void TileProcessor::StartProcessing(string outputDirectory)
{
    _pipeline.AddProcessingStep(new TileProducerStep(_mainRasterMap, _tileGrid));
    _pipeline.AddProcessingStep(new TileGpuTransferStep());
    _pipeline.AddProcessingStep(new TileWriterStep(outputDirectory));
    _pipeline.StartProcessing();
}
