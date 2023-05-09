
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include "Board.h"
#include "Hex.h"

#include <cmath>

int main()
{
    const int screenWidth = 1300;
    const int screenHeight = 1000;

    sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "Hex Board");

    std::vector< sf::Vector3i> neighbours = std::vector<sf::Vector3i>();
    Board grid(13, 21, 0.8f);

    while (window.isOpen())
    {

        window.clear();
        for (auto& pair : grid.hexDict) {
            Hex& hexagon = pair.second;
            window.draw(hexagon.shape);
        }
        window.display();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                for (auto& pair : grid.hexDict) {
                    Hex& hexagon = pair.second;

                    /*const float hexRadius = hexagon.getRadius();
                    sf::Vector2f hexOrigin = hexagon.getOrigin();
                    float distance = std::sqrt(
                        std::pow(mousePosition.x - hexOrigin.x, 2) +
                        std::pow(mousePosition.y - hexOrigin.y, 2)
                    );*/

                    if (hexagon.shape.getGlobalBounds().contains(mousePosition.x, mousePosition.y)) {

                        hexagon.highlight = false;

                        for (sf::Vector3i neighbour : neighbours)
                        {
                            grid.hexDict[neighbour].disableHighlight();
                        }

                        neighbours = grid.GetNeighbours(hexagon.cubeCoords);

                        for (sf::Vector3i neighbour : neighbours)
                        {
                            grid.hexDict[neighbour].enableHighlight();
                        }

                        std::cout << "q: " << hexagon.cubeCoords.x << " r: " << hexagon.cubeCoords.y << " s: " << hexagon.cubeCoords.z << "\n";
                    }

                    
                }
            }
            if (event.type == sf::Event::Closed)
                window.close();
        }
    }
    return 0;
}

