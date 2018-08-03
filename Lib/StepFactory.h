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
#include "AffineTransform.h"

class StepFactory
{
public:
    StepFactory();
    virtual ~StepFactory();

    std::shared_ptr<ProcessingPipeline> PipelineFor(const Settings& settings);
    std::shared_ptr<ProcessingStep> StepFor(const Settings& settings, const StepSettings& stepSettings);

private:
    class StepCreator
    {
    public:
        using FunctionType = std::function< std::shared_ptr<ProcessingStep>(const Settings&, const StepSettings&) >;

        StepCreator(std::string name, FunctionType createFunction)
                    :	_name(name),
                        _createFunction(createFunction)
        {
        }

        bool IsStepFor(const StepSettings& stepSettings)
        {
            return _name == stepSettings.Type();
        }

        std::shared_ptr<ProcessingStep> Create(const Settings& settings, const StepSettings& stepSettings)
        {
            return _createFunction(settings, stepSettings);
        }

    private:
        std::string _name;
        FunctionType _createFunction;
    };

private:
    AffineTransform GetAffineTransform(const Settings& settings);
    std::shared_ptr<VectorFile> LoadVectorFile(const Settings& settings, const StepSettings& stepSettings);

private:
    Settings _settings;
    std::shared_ptr<GeoMap> _mainRasterMap;
    std::vector< StepCreator > _steps;
};

#endif // STEPFACTORY_H
