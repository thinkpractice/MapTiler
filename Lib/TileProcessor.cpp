#include "TileProcessor.h"
#include "TileProducerStep.h"
#include "TileWriterStep.h"
#include "TileGpuTransferStep.h"
#include "TileDownloadStep.h"
#include "AddMetadataStep.h"
#include "MappedVectorFile.h"

TileProcessor::TileProcessor(shared_ptr<GeoMap> mainRasterMap, const Area& areaToProcess, const MapTilerSettings& settings)
                :	_mainRasterMap(mainRasterMap),
                    _areaToProcess(areaToProcess),
                    _settings(settings)
{
}

TileProcessor::~TileProcessor()
{
}

void TileProcessor::AddRasterMap(string mapName, shared_ptr<GeoMap> map)
{
	_rasterMaps[mapName] = map;
}

void TileProcessor::StartProcessing()
{
    _pipeline.AddProcessingStep(make_shared<TileProducerStep>(_mainRasterMap->RectForArea(_areaToProcess), _areaToProcess, _settings.tileWidth, _settings.tileHeight));
    _pipeline.AddProcessingStep(make_shared<TileDownloadStep>("rgb", _mainRasterMap));
	for (auto mapPair : _rasterMaps)
		_pipeline.AddProcessingStep(make_shared<TileDownloadStep>(mapPair.first, mapPair.second));

    for (auto metadataPair : _settings.metadataFilenames)
    {
        auto metadataSettings = metadataPair.second;
        _pipeline.AddProcessingStep(make_shared<AddMetadataStep>(metadataPair.first, make_shared<MappedVectorFile>(metadataSettings.metadataFilename, _mainRasterMap->ProjectionReference(), _mainRasterMap->MapTransform()), metadataSettings.layerIndex));
    }

    if (_settings.metadataFilenames.count("polygons") > 0)
        _pipeline.AddProcessingStep(make_shared<TileGpuTransferStep>(_settings.tileWidth, _settings.tileHeight));
    _pipeline.AddProcessingStep(make_shared<TileWriterStep>(_settings.targetDirectory));
    _pipeline.StartProcessing();
}
