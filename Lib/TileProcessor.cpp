#include "TileProcessor.h"
#include "TileProducerStep.h"
#include "TileWriterStep.h"
#include "TileGpuTransferStep.h"
#include "MappedVectorFile.h"

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

void TileProcessor::StartProcessing(string outputDirectory, string polygonFilename)
{
    _pipeline.AddProcessingStep(make_shared<TileProducerStep>(_mainRasterMap, _tileGrid));
    _pipeline.AddProcessingStep(make_shared<TileGpuTransferStep>(make_shared<MappedVectorFile>(polygonFilename, _mainRasterMap->ProjectionReference(), _mainRasterMap->MapTransform()),1));
    _pipeline.AddProcessingStep(make_shared<TileWriterStep>(outputDirectory));
    _pipeline.StartProcessing();
}
