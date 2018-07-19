#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>
#include <functional>
#include <memory>
#include "GeoMap.h"

class Utils
{
    public:
        static std::vector<std::string> SplitKeyValuePair(const char* keyValueString);
        static std::string GetKeyType(std::string key);
        static std::string UUID();
        static void TimeIt(std::function<void()> function);
        static std::string GetFileExtension(const std::string& fileName);
        static std::shared_ptr<GeoMap> LoadRasterMap(std::string layerUrl, int layerIndex);
};

#endif
