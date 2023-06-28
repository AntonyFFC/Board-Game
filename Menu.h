#pragma once
#include <SFML/Graphics.hpp>

class Menu {
public:
    Menu();

    void draw(sf::RenderWindow& window);

    void handleInput(sf::Event event);

    int getSelectedItem();
};
