#include "StepFactory.h"
#include "TileProducerStep.h"
#include "AddMetadataStep.h"
#include "TileFilterStep.h"
#include "TileDownloadStep.h"
#include "TileGpuTransferStep.h"
#include "TileWriterStep.h"

StepFactory::StepFactory()
{
    _steps =
    {
        {
            "TileProducerStep",
            std::shared_ptr<ProcessingStep> (const StepSettings& stepSettings)
            {
                return std::make_shared<TileProducerStep>(_mainRasterMap, _mainRasterMap->RectForArea(_areaToProcess), _areaToProcess, _settings.tileWidth, _settings.tileHeight))
            }
        },
        {
            "AddMetadataStep",
            std::shared_ptr<ProcessingStep> (const StepSettings& stepSettings)
            {
                return std::make_shared<AddMetadataStep>(stepSettings.LayerName(), LoadVectorFile(stepSettings));
            }
        },
        {
            "TileFilterStep",
            std::shared_ptr<ProcessingStep> (const StepSettings& stepSettings)
            {
                return std::make_shared<TileFilterStep>();
            }
        },
        {
            "TileDownloadStep",
            std::shared_ptr<ProcessingStep> (const StepSettings& stepSettings)
            {
                return std::make_shared<TileDownloadStep>(stepSettings.LayerName(), LoadRasterMap(stepSettings.LayerUrl()));
            }
        },
        {
            "TileGpuTransferStep",
            std::shared_ptr<ProcessingStep> (const StepSettings& stepSettings)
            {
                return std::make_shared<TileGpuTransferStep>(stepSettings.TileWidth(), stepSettings.TileHeight());
            }
        },
        {
            "TileWriterStep",
            std::shared_ptr<ProcessingStep> (const StepSettings& stepSettings)
            {
                return std::make_shared<TileWriterStep>(stepSettings.OutputDirectory());
            }
        }
    }
}

StepFactory::~StepFactory()
{
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

std::shared_ptr<GeoMap> StepFactory::LoadRasterMap(std::string rasterUrl)
{

}

std::shared_ptr<VectorFile> StepFactory::LoadVectorFile(const StepSettings& stepSettings)
{
    return std::make_shared<MappedVectorFile>(stepSettings.LayerUrl(), _mainRasterMap->ProjectionReference(), _mainRasterMap->MapTransform()), stepSettings.LayerIndex())
}
