#include "TileProcessor.h"
#include "TileProducerStep.h"
#include "TileWriterStep.h"
#include "TileGpuTransferStep.h"
#include "TileDownloadStep.h"
#include "MappedVectorFile.h"

TileProcessor::TileProcessor(shared_ptr<GeoMap> mainRasterMap, int tileWidth, int tileHeight)
                :   TileProcessor(mainRasterMap, mainRasterMap->GetMapArea(), tileWidth, tileHeight)
{
}

TileProcessor::TileProcessor(shared_ptr<GeoMap> mainRasterMap, const Area& areaToProcess, int tileWidth, int tileHeight)
                :   _mainRasterMap(mainRasterMap),
                    _tileGrid(_mainRasterMap->RectForArea(areaToProcess), areaToProcess, tileWidth, tileHeight)
{
}

TileProcessor::~TileProcessor()
{
}

void TileProcessor::AddRasterMap(string mapName, shared_ptr<GeoMap> map)
{
	_rasterMaps[mapName] = map;
}

void TileProcessor::StartProcessing(string outputDirectory, string polygonFilename)
{
    _pipeline.AddProcessingStep(make_shared<TileProducerStep>(_tileGrid));
    _pipeline.AddProcessingStep(make_shared<TileDownloadStep>("rgb", _mainRasterMap));
	for (auto mapPair : _rasterMaps)
		_pipeline.AddProcessingStep(make_shared<TileDownloadStep>(mapPair.first, mapPair.second));
		
    _pipeline.AddProcessingStep(make_shared<TileGpuTransferStep>(make_shared<MappedVectorFile>(polygonFilename, _mainRasterMap->ProjectionReference(), _mainRasterMap->MapTransform()), 1, _tileGrid.TileWidth(), _tileGrid.TileHeight()));
    _pipeline.AddProcessingStep(make_shared<TileWriterStep>(outputDirectory));
    _pipeline.StartProcessing();
}
