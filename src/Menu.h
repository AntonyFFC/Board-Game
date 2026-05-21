#pragma once
#include <SFML/Graphics.hpp>
#include "Globals.h"
#include "GUI.h"
#include "Armory.h"
#include "Button.h"
#include "Shop.h"

class Menu {
public:
    Menu(const int screenWidth = 1300, const int screenHeight = 1000);
    ~Menu();

    void start();

    void draw();

    void handleInput(sf::Event event);

    int getSelectedItem();

    void callSelected(int selected);

private:
    void display();

    void initializeButtons();

    std::vector<std::string> buttonLabels;
    std::vector<Button> buttons;
    int screenWidth;
    int screenHeight;
    float totalHeight;
    float startY;
    sf::RenderWindow* window;
    int selectedIndex;
    Gui* interface1;
    Shop* shop1;
    Armory* armory1;
    sf::Sprite backgroundSprite;
    sf::Texture backgroundTexture;
};
