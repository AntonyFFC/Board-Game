#include "SelectionManager.h"
#include "Board.h"
#include "Hex.h"
#include <SFML/Graphics.hpp>
#include <iostream>


void selectionManager::handleClick(Board &grid, sf::Vector2i mousePosition)
{

    for (auto& pair : grid.hexDict) {
        Hex& hexagon = pair.second;

        const float hexRadius = hexagon.getRadius();
        sf::Vector2f hexOrigin = hexagon.getOrigin();
        float distance = std::sqrt(
            std::pow(mousePosition.x - hexOrigin.x, 2) +
            std::pow(mousePosition.y - hexOrigin.y, 2)
        );

        if (distance < hexRadius) {

            hexagon.disableHighlight();
            //hexagon.setRock();

            for (std::tuple<int, int, int> neighbour : neighbours)
            {
                grid.hexDict[neighbour].disableHighlight();
            }

            neighbours = grid.GetNeighbours(hexagon.cubeCoords);

            for (std::tuple<int, int, int> neighbour : neighbours)
            {
                grid.hexDict[neighbour].enableHighlight();             
            }

            std::cout << "{" << std::get<0>(hexagon.cubeCoords) << ", " << std::get<1>(hexagon.cubeCoords) << ", " << std::get<2>(hexagon.cubeCoords) << "}, ";
        }


    }
    
}