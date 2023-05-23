#include "SelectionManager.h"
#include "Board.h"
#include "Hex.h"
#include <SFML/Graphics.hpp>
#include <iostream>


void selectionManager::handleClick(Board &grid, sf::Vector2i mousePosition)
{

    for (auto& pair : grid.hexDict) {
        Hex* hexagon = pair.second;

        if (hexagon->isClicked(mousePosition)){

            hexagon->setHighlight(false);
            //hexagon.setRock();

            for (std::tuple<int, int, int> neighbour : neighbours)
            {
                grid.hexDict[neighbour]->setHighlight(false);
            }

            neighbours = grid.GetNeighbours(hexagon->getCubeCoords());

            for (std::tuple<int, int, int> neighbour : neighbours)
            {
                grid.hexDict[neighbour]->setHighlight(true);             
            }

            std::cout << "{" << std::get<0>(hexagon->getCubeCoords()) << ", " << std::get<1>(hexagon->getCubeCoords()) << ", " << std::get<2>(hexagon->getCubeCoords()) << "}, ";
        }


    }
    
}