#include "ProcessingStep.h"

ProcessingStep::ProcessingStep(ProcessingType type)
                    :   _type(type)
{
}

ProcessingStep::~ProcessingStep()
{
}

ProcessingStep::ProcessingType ProcessingStep::Type()
{
    return _type;
}

