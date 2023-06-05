#include "SelectionManager.h"
#include "Board.h"
#include "Hex.h"
#include <SFML/Graphics.hpp>
#include <iostream>


void selectionManager::handleClick(Board &grid, sf::Vector2i mousePosition)
{
    std::string Name = "Mirmi³";
    //std::vector<Pawn*> pawns;
    //Pawn* p1 = new Pawn(Name, 1, 0, 5, 4, { 2,1 }, 7, 510.f, 504.0f);
    //pawns.push_back(p1);
    //Pawn* p2 = new Pawn(Name, 0, 1, 5, 4, { 2,2 }, 7, 510.f, 504.0f);
    //pawns.push_back(p2);
    //Pawn* p3 = new Pawn(Name, 1, 1, 5, 4, { 2,1 }, 7, 510.f, 504.0f);
    //pawns.push_back(p3);
    //Pawn* p4 = new Pawn(Name, 0, 0, 5, 4, { 1,1 }, 7, 510.f, 504.0f);
    //pawns.push_back(p4);
    //Pawn* p5 = new Pawn(Name, 1, 1, 5, 4, { 1,2 }, 7, 510.f, 504.0f);
    //pawns.push_back(p5);
    //Pawn* p6 = new Pawn(Name, 1, 0, 5, 4, { 2,2 }, 7, 510.f, 504.0f); //to ca³e najwiêcej zabiera czasu
    //pawns.push_back(p6);
    for (auto& pair : grid.hexDict) {
        Hex* hexagon = pair.second;

        if (hexagon->isClicked(mousePosition)){

            hexagon->setHighlight(false);
            //hexagon.setRock();

            for (std::tuple<int, int, int> neighbour : neighbours)
            {
                //grid.hexDict[neighbour]->setPawn(false);
                grid.hexDict[neighbour]->setHighlight(false);
            }

            neighbours = grid.GetNeighbours(hexagon->getCubeCoords());

            int i = 0;
            for (std::tuple<int, int, int> neighbour : neighbours)
            {
                //grid.hexDict[neighbour]->setPawn(true, pawns[i]);
                grid.hexDict[neighbour]->setHighlight(true); 
                i++;
            }

            std::cout << "{" << std::get<0>(hexagon->getCubeCoords()) << ", " << std::get<1>(hexagon->getCubeCoords()) << ", " << std::get<2>(hexagon->getCubeCoords()) << "}, ";
        }


    }
    
}