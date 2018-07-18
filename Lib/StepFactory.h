#ifndef STEPFACTORY_H
#define STEPFACTORY_H

#include <memory>
#include <functional>
#include "StepSettings.h"

class StepFactory
{
public:
    StepFactory();
    virtual ~StepFactory();

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
    std::shared_ptr<GeoMap> LoadRasterMap(std::string rasterUrl);
    std::shared_ptr<VectorFile> LoadVectorMap(std::string vectorUrl);

private:
    std::vector< StepCreator > _steps;
};

#endif // STEPFACTORY_H
