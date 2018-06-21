#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>
#include <functional>

using namespace std;

class Utils
{
    public:
        static vector<string> SplitKeyValuePair(const char* keyValueString);
        static string GetKeyType(string key);
        static string UUID();
        static void TimeIt(function<void()> function);
        static string GetFileExtension(const string& fileName);
};

#endif
