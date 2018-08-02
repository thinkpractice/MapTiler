#include "Utils.h"
#include <regex>
#include <uuid/uuid.h>
#include <chrono>
#include <iostream>
#include "GeoMapProvider.h"

std::vector<std::string> Utils::SplitKeyValuePair(const char* keyValueString)
{
    std::vector <std::string> matchResults;

    std::regex re(R"((\w+)=(.+))");
    std::cmatch matches;

    if (std::regex_match(keyValueString, matches, re))
    {
        for (const auto& matchResult : matches)
        {
            matchResults.push_back(string(matchResult));
        }
    }

    return matchResults;
}

std::string Utils::GetKeyType(std::string key)
{
    std::regex re(R"((\w+_\d+_)(\w+))");
    std::smatch matches;
    if (std::regex_match(key, matches, re))
    {
        return matches[2];
    }
    return "";
}

std::string Utils::UUID()
{
    uuid_t uuidObj;
    uuid_generate(uuidObj);

    char uuid_str[37];
    uuid_unparse_lower(uuidObj, uuid_str);
    return string(uuid_str);
}

void Utils::TimeIt(std::function<void()> function)
{
    auto start = chrono::system_clock::now();
    function();
    auto end = chrono::system_clock::now();

    std::chrono::duration<double> elapsed_seconds = end-start;
    std::cout << "Elapsed seconds " << std::elapsed_seconds.count() <<endl;
}

std::string Utils::GetFileExtension(const std::string& fileName)
{
    if(fileName.find_last_of(".") != string::npos)
        return fileName.substr(fileName.find_last_of(".") + 1);
    return "";
}

std::shared_ptr<GeoMap> Utils::LoadRasterMap(std::string layerUrl, int layerIndex)
{
    GeoMapProvider mapProvider(layerUrl);
    if (mapProvider.Maps().size() == 0)
    {
        std::cerr << "No maps at url/in file" << std::endl;
        return nullptr;
    }

    if (mapProvider.Maps().size() >= 1)
    {
        std::cout << "Multiple Maps found at url, continuing with map at layerIndex: " << layerIndex << std::endl;
    }
    return mapProvider.Maps()[layerIndex];
}

std::shared_ptr<VectorFile> Utils::LoadVectorFile(std::string filename, VectorFile::OpenMode mode)
{
    return std::make_shared<VectorFile>(filename, mode);
}
