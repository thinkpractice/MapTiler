#ifndef MENU_H
#define MENU_H

#include <string>
#include <vector>
#include <functional>
#include <iostream>

template <class T>
class Menu
{
    public:
        static T ShowMenu(std::vector<T> menuOptions, std::function<std::string(int, T)> convertToString);
};

template <class T>
T Menu<T>::ShowMenu(std::vector<T> menuOptions, std::function<string(int, T)> convertToString)
{
    int i = 0;
    for (auto& menuOption : menuOptions)
    {
        std::cout << convertToString(i, menuOption) << std::endl;
        i++;
    }

    std::string input;
    int chosenIndex = -1;
    do
    {
        std::cout << "Choose an option" << std::endl;
        getline(std::cin, input);
        chosenIndex = stoi(input);
    }
    while (chosenIndex < 0 || chosenIndex >= menuOptions.size());

    return menuOptions[chosenIndex];
}

#endif
