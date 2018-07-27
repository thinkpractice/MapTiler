#include "Utils.h"
#include <regex>
#include <uuid/uuid.h>
#include <chrono>
#include <iostream>
#include "GeoMapProvider.h"

std::vector<std::string> Utils::SplitKeyValuePair(const char* keyValueString)
{
    vector <string> matchResults;

    regex re(R"((\w+)=(.+))");
    cmatch matches;

    if (regex_match(keyValueString, matches, re))
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
    regex re(R"((\w+_\d+_)(\w+))");
    smatch matches;
    if (regex_match(key, matches, re))
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

    chrono::duration<double> elapsed_seconds = end-start;
    cout << "Elapsed seconds " << elapsed_seconds.count() <<endl;
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
        cerr << "No maps at url/in file" << endl;
        return nullptr;
    }

    if (mapProvider.Maps().size() >= 1)
    {
        cout << "Multiple Maps found at url, continuing with map at layerIndex: " << layerIndex << endl;
    }
    return mapProvider.Maps()[layerIndex];
}

std::shared_ptr<VectorFile> Utils::LoadVectorFile(std::string filename, VectorFile::OpenMode mode)
{
    return std::make_shared<VectorFile>(filename, mode);
}
