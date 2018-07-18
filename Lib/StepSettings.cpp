#include "StepSettings.h"

StepSettings::StepSettings()
{
}

StepSettings::~StepSettings()
{
}

std::string StepSettings::Name()
{
    return _name;
}

void StepSettings::SetName(std::string name)
{
    _name = name;
}

std::string StepSettings::Type()
{
    return _type;
}

void StepSettings::SetType(std::string type)
{
    _type = type;
}
