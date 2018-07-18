#ifndef STEPSETTINGS_H
#define STEPSETTINGS_H

#include <string>

class StepSettings
{
public:
    StepSettings();
    virtual ~StepSettings();

    std::string Name();
    void SetName(std::string name);

    std::string Type();
    void SetType(std::string type);

private:
    std::string _name;
    std::string _type;
};

#endif // STEPSETTINGS_H
