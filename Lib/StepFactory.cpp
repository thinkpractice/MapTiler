#include "StepFactory.h"
#include "GridProducerStep.h"
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
                std::cerr << "TileProducerStep type will be replaced by GridProducerStep, please use this step type in future versions" << endl;
                return std::make_unique<GridProducerStep>(settings.MainRasterUrl(), settings.MainRasterLayerIndex(), settings.ChosenArea(), stepSettings.TileWidth(), stepSettings.TileHeight(), stepSettings.PersistenceUrl());
            }
        },
        {
            "GridProducerStep",
            [&](const Settings& settings, const StepSettings& stepSettings)
            {
                return std::make_unique<GridProducerStep>(settings.MainRasterUrl(), settings.MainRasterLayerIndex(), settings.ChosenArea(), stepSettings.TileWidth(), stepSettings.TileHeight(), stepSettings.PersistenceUrl());
            }
        },
        {
            "AddMetadataStep",
            [&] (const Settings& settings, const StepSettings& stepSettings)
            {
                return std::make_unique<AddMetadataStep>(stepSettings.LayerName(), LoadVectorFile(settings, stepSettings), stepSettings.LayerIndex(), stepSettings.PersistenceUrl(), stepSettings.PersistenceLayerName());
            }
        },
        {
            "TileFilterStep",
            [&](const Settings& settings, const StepSettings& stepSettings)
            {
                return std::make_unique<TileFilterStep>(stepSettings.LayerName());
            }
        },
        {
            "TileDownloadStep",
            [&] (const Settings& settings, const StepSettings& stepSettings)
            {
                return std::make_unique<TileDownloadStep>(stepSettings.LayerName(), stepSettings.LayerUrl(), stepSettings.LayerIndex(), stepSettings.MapYear(), stepSettings.DriverOptions());
            }
        },
        {
            "TileGpuTransferStep",
            [&] (const Settings& settings, const StepSettings& stepSettings)
            {
                return std::make_unique<TileGpuTransferStep>(GetAffineTransform(settings), stepSettings.MaskingLayerName(), stepSettings.TileWidth(), stepSettings.TileHeight());
            }
        },
        {
            "TileWriterStep",
            [&] (const Settings& settings, const StepSettings& stepSettings)
            {
                return std::make_unique<TileWriterStep>(stepSettings.OutputDirectory(), stepSettings.PersistenceUrl(), stepSettings.DriverName(), stepSettings.EpsgFormat(), stepSettings.FileExtension(), stepSettings.DriverOptions());
            }
        }
    };
}

StepFactory::~StepFactory()
{
}

std::unique_ptr<ProcessingPipeline> StepFactory::PipelineFor(const Settings &settings)
{
    auto pipeline = make_unique<ProcessingPipeline>();
    for (auto& stepSettings : settings.StepSettingsCollection())
    {
        auto processingStep = StepFor(settings, stepSettings);
        if (!processingStep)
            continue;
        pipeline->AddProcessingStep(std::move(processingStep));
    }
    return pipeline;
}

std::unique_ptr<ProcessingStep> StepFactory::StepFor(const Settings& settings, const StepSettings &stepSettings)
{
    for (auto& step : _steps)
    {
        if (step.IsStepFor(stepSettings))
            return step.Create(settings, stepSettings);
    }
    return nullptr;
}

AffineTransform StepFactory::GetAffineTransform(const Settings& settings)
{
    auto mainRasterMap = Utils::LoadRasterMap(settings.MainRasterUrl(), settings.MainRasterLayerIndex());
    return mainRasterMap->MapTransform();
}

std::unique_ptr<VectorFile> StepFactory::LoadVectorFile(const Settings& settings, const StepSettings& stepSettings)
{
    auto mainRasterMap = Utils::LoadRasterMap(settings.MainRasterUrl(), settings.MainRasterLayerIndex());
    return std::make_unique<MappedVectorFile>(stepSettings.LayerUrl(), mainRasterMap->ProjectionReference());
}
