#ifndef STEPFACTORY_H
#define STEPFACTORY_H

#include <memory>
#include <functional>
#include "StepSettings.h"
#include "ProcessingStep.h"
#include "ProcessingPipeline.h"
#include "GeoMap.h"
#include "VectorFile.h"
#include "Settings.h"

class StepFactory
{
public:
    StepFactory(const Settings& settings);
    virtual ~StepFactory();

    ProcessingPipeline PipelineFor(const Settings& settings);
    std::shared_ptr<ProcessingStep> StepFor(const StepSettings& stepSettings);

private:
    class StepCreator
    {
    public:
        using FunctionType = std::function< std::shared_ptr<ProcessingStep>(const StepSettings&) >;

        StepCreator(std::string name, FunctionType createFunction)
                    :	_name(name),
                        _createFunction(createFunction)
        {
        }

        bool IsStepFor(const StepSettings& stepSettings)
        {
            return _name == stepSettings.Name();
        }

        std::shared_ptr<ProcessingStep> Create(const StepSettings& stepSettings)
        {
            return _createFunction(stepSettings);
        }

    private:
        std::string _name;
        FunctionType _createFunction;
    };

private:
    std::shared_ptr<GeoMap> LoadRasterMap(std::string layerUrl, int layerIndex);
    std::shared_ptr<VectorFile> LoadVectorFile(const StepSettings& stepSettings);

private:
    Settings _settings;
    std::shared_ptr<GeoMap> _mainRasterMap;
    std::vector< StepCreator > _steps;
};

#endif // STEPFACTORY_H
