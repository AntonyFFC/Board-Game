
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Board.h"

int main()
{
    const int screenWidth = 1300;
    const int screenHeight = 1000;
    sf::Color picked(102, 255, 153);

    sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "Hex Board");

    Board grid;

    while (window.isOpen())
    {

        window.clear();
        for (sf::ConvexShape& hexagon : grid.hexagons) {
            window.draw(hexagon);
        }
        window.display();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                for (sf::ConvexShape& hexagon : grid.hexagons) {
                    if (hexagon.getGlobalBounds().contains(mousePosition.x, mousePosition.y)) {
                        hexagon.setFillColor(picked);
                        hexagon.setOutlineColor(sf::Color::Yellow);
                    }
                }
            }
            if (event.type == sf::Event::Closed)
                window.close();
        }
    }
    return 0;
}

