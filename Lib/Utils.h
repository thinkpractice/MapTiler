#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>
#include <functional>
#include <memory>
#include "GeoMap.h"
#include "VectorFile.h"

class Utils
{
    public:
        static std::vector<std::string> SplitKeyValuePair(const char* keyValueString);
        static std::string GetKeyType(std::string key);
        static std::string UUID();
        static void TimeIt(std::function<void()> function);
        static bool FileExists(const std::string& path);
        static std::string GetFileExtension(const std::string& fileName);
        static std::shared_ptr<GeoMap> LoadRasterMap(std::string layerUrl, int layerIndex);
        static std::shared_ptr<VectorFile> LoadVectorFile(std::string filename, VectorFile::OpenMode mode = VectorFile::OpenMode::ReadOnly);
};

#endif
