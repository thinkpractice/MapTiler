#include "Utils.h"
#include <regex>
#include <uuid/uuid.h>

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

