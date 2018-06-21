#include "Utils.h"
#include <regex>
#include <uuid/uuid.h>
#include <chrono>
#include <iostream>

vector<string> Utils::SplitKeyValuePair(const char* keyValueString)
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

string Utils::GetKeyType(string key)
{
    regex re(R"((\w+_\d+_)(\w+))");
    smatch matches;
    if (regex_match(key, matches, re))
    {
        return matches[2];
    }
    return "";
}

string Utils::UUID()
{
    uuid_t uuidObj;
    uuid_generate(uuidObj);

    char uuid_str[37];
    uuid_unparse_lower(uuidObj, uuid_str);
    return string(uuid_str);
}

void Utils::TimeIt(function<void()> function)
{
    auto start = chrono::system_clock::now();
    function();
    auto end = chrono::system_clock::now();

    chrono::duration<double> elapsed_seconds = end-start;
    cout << "Elapsed seconds " << elapsed_seconds.count() <<endl;
}

string Utils::GetFileExtension(const string& fileName)
{
    if(fileName.find_last_of(".") != string::npos)
        return fileName.substr(fileName.find_last_of(".") + 1);
    return "";
}
