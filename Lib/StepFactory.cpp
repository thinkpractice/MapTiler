#include "StepFactory.h"
#include "TileProducerStep.h"
#include "AddMetadataStep.h"
#include "TileFilterStep.h"
#include "TileDownloadStep.h"
#include "TileGpuTransferStep.h"
#include "TileWriterStep.h"
#include "GeoMapProvider.h"
#include "MappedVectorFile.h"

StepFactory::StepFactory(const Settings& settings)
                :	_settings(settings)
{
    _mainRasterMap = LoadRasterMap(_settings.MainRasterUrl(), _settings.MainRasterLayerIndex());
    _steps =
    {
        {
            "TileProducerStep",
            [&](const StepSettings& stepSettings)
            {
                return std::make_shared<TileProducerStep>(_mainRasterMap, _settings.ChosenArea(), stepSettings.TileWidth(), stepSettings.TileHeight());
            }
        },
        {
            "AddMetadataStep",
            [&] (const StepSettings& stepSettings)
            {
                return std::make_shared<AddMetadataStep>(stepSettings.LayerName(), LoadVectorFile(stepSettings), stepSettings.LayerIndex());
            }
        },
        {
            "TileFilterStep",
            [&](const StepSettings& stepSettings)
            {
                return std::make_shared<TileFilterStep>();
            }
        },
        {
            "TileDownloadStep",
            [&] (const StepSettings& stepSettings)
            {
                return std::make_shared<TileDownloadStep>(stepSettings.LayerName(), LoadRasterMap(stepSettings.LayerName(), stepSettings.LayerIndex()));
            }
        },
        {
            "TileGpuTransferStep",
            [&] (const StepSettings& stepSettings)
            {
                return std::make_shared<TileGpuTransferStep>(stepSettings.TileWidth(), stepSettings.TileHeight());
            }
        },
        {
            "TileWriterStep",
            [&] (const StepSettings& stepSettings)
            {
                return std::make_shared<TileWriterStep>(stepSettings.OutputDirectory());
            }
        }
    };
}

StepFactory::~StepFactory()
{
}

ProcessingPipeline StepFactory::PipelineFor(const Settings &settings)
{
    ProcessingPipeline pipeline;
    for (auto& stepSettings : settings.StepSettingsCollection())
    {
        pipeline.AddProcessingStep(StepFor(stepSettings));
    }
    return pipeline;
}

std::shared_ptr<ProcessingStep> StepFactory::StepFor(const StepSettings &stepSettings)
{
    for (auto& step : _steps)
    {
        if (step.IsStepFor(stepSettings))
            return step.Create(stepSettings);
    }
    return nullptr;
}

std::shared_ptr<GeoMap> StepFactory::LoadRasterMap(std::string layerUrl, int layerIndex)
{
    GeoMapProvider mapProvider(layerUrl);
    if (mapProvider.Maps().size() == 0)
    {
        cerr << "No maps at url/in file" << endl;
        return nullptr;
    }

    if (mapProvider.Maps().size() >= 1)
    {
        cout << "Multiple Maps found at url, continuing with map at layerIndex: " << layerIndex << endl;
    }
    return mapProvider.Maps()[layerIndex];
}

std::shared_ptr<VectorFile> StepFactory::LoadVectorFile(const StepSettings& stepSettings)
{
    return std::make_shared<MappedVectorFile>(stepSettings.LayerUrl(), _mainRasterMap->ProjectionReference(), _mainRasterMap->MapTransform());
}
