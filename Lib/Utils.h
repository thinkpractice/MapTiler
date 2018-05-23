#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>

using namespace std;

class Utils
{
    public:
        static vector<string> SplitKeyValuePair(const char* keyValueString);
        static string GetKeyType(string key);
        static string UUID();
};

#endif
