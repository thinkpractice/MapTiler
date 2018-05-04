#include "Menu.h"
#include <iostream>

template <class T>
T Menu::ShowMenu(vector<T> menuOptions, function<string(int, T)> convertToString)
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
