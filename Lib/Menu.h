#ifndef MENU_H
#define MENU_H

#include <string>
#include <vector>
#include <functional>
#include <iostream>

using namespace std;

template <class T>
class Menu
{
    public:
        static T ShowMenu(vector<T> menuOptions, function<string(int, T)> convertToString);
};

template <class T>
T Menu<T>::ShowMenu(vector<T> menuOptions, function<string(int, T)> convertToString)
{
    int i = 0;
    for (auto& menuOption : menuOptions)
    {
        cout << convertToString(i, menuOption) << endl;
        i++;
    }

    string input;
    int chosenIndex = -1;
    do
    {
        cout << "Choose an option" << endl;
        getline(cin, input);
        chosenIndex = stoi(input);
    }
    while (chosenIndex < 0 || chosenIndex >= menuOptions.size());

    return menuOptions[chosenIndex];
}

#endif
