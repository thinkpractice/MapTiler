#include "StepFactory.h"

StepFactory::StepFactory()
{
    _steps =
    {

    }
}

StepFactory::~StepFactory()
{
}

std::shared_ptr<ProcessingStep> StepFactory::StepFor(const StepSettings &stepSettings)
{

    return nullptr;
}
