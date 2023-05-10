
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include "Board.h"
#include "Hex.h"
#include <tuple>
#include <cmath>
#include "SelectionManager.h"

int main()
{
    const int screenWidth = 1300;
    const int screenHeight = 1000;
    sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "Hex Board");

    selectionManager sM;
    Board grid(13, 19, 0.8f);

    while (window.isOpen())
    {

        window.clear();
        for (auto& pair : grid.hexDict) {
            Hex& hexagon = pair.second;
            window.draw(hexagon.shape);
            /*window.draw(hexagon.rockShape);*/
        }
        window.display();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                sM.handleClick(grid, mousePosition);
            }
            if (event.type == sf::Event::Closed)
                window.close();
        }
    }
    return 0;
}

