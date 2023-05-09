
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Board.h"
#include "Hex.h"
#include <tuple>

int main()
{
    const int screenWidth = 1300;
    const int screenHeight = 1000;

    sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "Hex Board");

    std::vector< std::tuple<int, int, int>> neighbours = std::vector< std::tuple<int, int, int>>();
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
                    if (hexagon.shape.getGlobalBounds().contains(mousePosition.x, mousePosition.y)) {

                        hexagon.highlight = false;

                        for (std::tuple<int, int, int> neighbour : neighbours)
                        {
                            grid.hexDict[neighbour].disableHighlight();
                        }

                        neighbours = grid.GetNeighbours(hexagon.cubeCoords);

                        for (std::tuple<int, int, int> neighbour : neighbours)
                        {
                            grid.hexDict[neighbour].enableHighlight();
                        }

                        std::cout << "q: " << std::get<0>(hexagon.cubeCoords) << " r: " << std::get<1>(hexagon.cubeCoords) << " s: " << std::get<2>(hexagon.cubeCoords) << "\n";
                    }

                    
                }
            }
            if (event.type == sf::Event::Closed)
                window.close();
        }
    }
    return 0;
}

