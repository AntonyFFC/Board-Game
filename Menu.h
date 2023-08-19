#pragma once
#include <SFML/Graphics.hpp>
#include "Globals.h"
#include "GUI.h"

class Menu {
public:
    Menu(const int screenWidth = 1300, const int screenHeight = 1000);
    ~Menu();

    void start();

    void draw();

    void handleInput(sf::Event event);

    int getSelectedItem();
private:
    std::vector<std::string> buttonLabels;
    int screenWidth;
    int screenHeight;
    sf::RenderWindow* window;
    int selected;
    Gui interface1;
};
