#ifndef UTILS_H
#define UTILS_H

#include <vector>

using namespace std;

class Utils
{
    public:
        static vector<string> splitKeyValuePair(const char* keyValueString);
};

#endif
