#include "StepFactory.h"
#include "TileProducerStep.h"
#include "AddMetadataStep.h"
#include "TileFilterStep.h"
#include "TileDownloadStep.h"
#include "TileGpuTransferStep.h"
#include "TileWriterStep.h"
#include "GeoMapProvider.h"
#include "MappedVectorFile.h"
#include "Utils.h"

StepFactory::StepFactory()
{
    _steps =
    {
        {
            "TileProducerStep",
            [&](const Settings& settings, const StepSettings& stepSettings)
            {
                return std::make_shared<TileProducerStep>(settings.MainRasterUrl(), settings.MainRasterLayerIndex(), settings.ChosenArea(), stepSettings.TileWidth(), stepSettings.TileHeight(), stepSettings.PersistenceUrl(), stepSettings.PersistenceLayerName());
            }
        },
        {
            "AddMetadataStep",
            [&] (const Settings& settings, const StepSettings& stepSettings)
            {
                return std::make_shared<AddMetadataStep>(stepSettings.LayerName(), LoadVectorFile(settings, stepSettings), stepSettings.LayerIndex());
            }
        },
        {
            "TileFilterStep",
            [&](const Settings& settings, const StepSettings& stepSettings)
            {
                return std::make_shared<TileFilterStep>();
            }
        },
        {
            "TileDownloadStep",
            [&] (const Settings& settings, const StepSettings& stepSettings)
            {
                return std::make_shared<TileDownloadStep>(stepSettings.LayerName(), stepSettings.LayerUrl(), stepSettings.LayerIndex());
            }
        },
        {
            "TileGpuTransferStep",
            [&] (const Settings& settings, const StepSettings& stepSettings)
            {
                return std::make_shared<TileGpuTransferStep>(stepSettings.TileWidth(), stepSettings.TileHeight());
            }
        },
        {
            "TileWriterStep",
            [&] (const Settings& settings, const StepSettings& stepSettings)
            {
                return std::make_shared<TileWriterStep>(stepSettings.OutputDirectory(), stepSettings.PersistenceUrl());
            }
        }
    };
}

StepFactory::~StepFactory()
{
}

std::shared_ptr<ProcessingPipeline> StepFactory::PipelineFor(const Settings &settings)
{
    auto pipeline = make_shared<ProcessingPipeline>();
    for (auto& stepSettings : settings.StepSettingsCollection())
    {
        auto processingStep = StepFor(settings, stepSettings);
        if (!processingStep)
            continue;
        pipeline->AddProcessingStep(processingStep);
    }
    return pipeline;
}

std::shared_ptr<ProcessingStep> StepFactory::StepFor(const Settings& settings, const StepSettings &stepSettings)
{
    for (auto& step : _steps)
    {
        if (step.IsStepFor(stepSettings))
            return step.Create(settings, stepSettings);
    }
    return nullptr;
}

std::shared_ptr<VectorFile> StepFactory::LoadVectorFile(const Settings& settings, const StepSettings& stepSettings)
{
    auto mainRasterMap = Utils::LoadRasterMap(settings.MainRasterUrl(), settings.MainRasterLayerIndex());
    return std::make_shared<MappedVectorFile>(stepSettings.LayerUrl(), mainRasterMap->ProjectionReference(), mainRasterMap->MapTransform());
}
