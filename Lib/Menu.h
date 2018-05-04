#ifndef MENU_H
#define MENU_H

#include <string>
#include <vector>
#include <functional>

using namespace std;

class Menu
{
    public:
        template <class T> static T ShowMenu(vector<T> menuOptions, function<string(int, T)> convertToString);

};

#endif
