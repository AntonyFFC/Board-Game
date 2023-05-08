
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Board.h"
#include "Hex.h"

int main()
{
    const int screenWidth = 1300;
    const int screenHeight = 1000;
    sf::Color picked(102, 255, 153);

    sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "Hex Board");

    Board grid(13, 21, 0.8f);

    while (window.isOpen())
    {

        window.clear();
        for (Hex hexagon : grid.hexagons) {
            window.draw(hexagon.shape);
        }
        window.display();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                for (Hex hexagon : grid.hexagons) {
                    if (hexagon.shape.getGlobalBounds().contains(mousePosition.x, mousePosition.y)) {
                        hexagon.shape.setFillColor(picked);
                        hexagon.shape.setOutlineColor(sf::Color::Yellow);
                        std::cout << "q: " << hexagon.x << " r: " << hexagon.y << " s: " << hexagon.z << "\n";
                    }
                }
            }
            if (event.type == sf::Event::Closed)
                window.close();
        }
    }
    return 0;
}

