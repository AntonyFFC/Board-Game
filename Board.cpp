#include "Board.h"
#include "Hex.h"
#include <SFML/Graphics.hpp>
#include <random>
//#include <iostream>

Board::Board(int Rows, int Columns, float HSize)
    :numRows(Rows), numCols(Columns), hexSize(HSize)
{
    numHexagons = numRows * numCols;
    vertSpacing = 86.6f;
    horizSpacing = 75.f;
    xOffset = 30.f;
    yOffset = 20.f;

    boardPreperation();

}

class DirectionVectors {
public:
    static std::vector<std::tuple<int, int, int>> directionVectors;

    static std::vector<std::tuple<int, int, int>> GetDirectionList() {
        return directionVectors;
    }
};

std::vector<std::tuple<int, int, int>> DirectionVectors::directionVectors = {
    {1, 0, -1}, {1, -1, 0}, {0, -1, 1},
    {-1, 0, 1}, {-1, 1, 0}, {0, 1, -1}
};

std::tuple<int, int, int> AddTuples(const std::tuple<int, int, int>& tuple1, const std::tuple<int, int, int>& tuple2) {
    int x1, y1, z1, x2, y2, z2;
    std::tie(x1, y1, z1) = tuple1;
    std::tie(x2, y2, z2) = tuple2;
    return std::make_tuple(x1 + x2, y1 + y2, z1 + z2);
}

std::vector <std::tuple<int, int, int>> Board::GetNeighbours(std::tuple<int, int, int> hexCoordinates)
{
    if (!hexDict.count(hexCoordinates))
        return std::vector<std::tuple<int, int, int>>();

    if (hexNeighboursDict.count(hexCoordinates))
        return hexNeighboursDict[hexCoordinates];

    hexNeighboursDict[hexCoordinates] = std::vector<std::tuple<int, int, int>>();
    for (const auto& direction : DirectionVectors::GetDirectionList()) //this adds the neighbours of the new hex to the dictionary
    {
        if (hexDict.count(AddTuples(hexCoordinates, direction)))
        {
            hexNeighboursDict[hexCoordinates].push_back(AddTuples(hexCoordinates, direction));
        }
    }

    return hexNeighboursDict[hexCoordinates];
}

std::vector < std::tuple<int, int, int>> Board::GetNeighboursIn(std::tuple<int, int, int> hexCoordinates, int distance)
{
    if (!hexDict.count(hexCoordinates))
        return std::vector<std::tuple<int, int, int>>();

    if (distance == 0)
        return std::vector<std::tuple<int, int, int>>{hexCoordinates};
    distance -= 1;

    std::vector < std::tuple<int, int, int>> Neighbours = GetNeighbours(hexCoordinates);
    std::vector < std::tuple<int, int, int>> thisNeighbours = Neighbours;
    for (std::tuple<int, int, int> neighbour : thisNeighbours)
    {
        std::vector < std::tuple<int, int, int>> newNeighbours = GetNeighboursIn(neighbour, distance);
        Neighbours.insert(Neighbours.end(), newNeighbours.begin(), newNeighbours.end());
    }
    return Neighbours;
}

class ObjectCoordinates {
public:
    static std::vector < std::vector<std::tuple<int, int, int>>> rockCoordinates;
    static std::vector<std::tuple<int, int, int>> deleteCoordinates;
    static std::vector<std::tuple<int, int, int>> startCoordinates;

    static std::vector<std::tuple<int, int, int>> getRockCoordinates() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, rockCoordinates.size() - 1);
        int random_index = dis(gen);
        return rockCoordinates[random_index];
    }

    static std::vector<std::tuple<int, int, int>> getdeleteCoordinates() {
        return deleteCoordinates;
    }

    static std::vector<std::tuple<int, int, int>> getStartCoordinates() {
        return startCoordinates;
    }
};

std::vector<std::vector<std::tuple<int, int, int>>> ObjectCoordinates::rockCoordinates = {
    {{1, -1, 0}, {1, 0, -1}, {-1, -1, 2}, {0, -2, 2}, {2, -3, 1}, {3, -3, 0}, {3, 0, -3}, {2, 1, -3}, {0, 2, -2}, {-1, 2, -1}, {-2, -3, 5}, {4, -6, 2}, {4, 2, -6}, {-2, 5, -3}},
    {{-2, 0, 2}, {-1, -2, 3}, {0, -2, 2}, {1, -4, 3}, {2, -4, 2}, {2, -3, 1}, {3, -3, 0}, {-1, 2, -1}, {0, 2, -2}, {0, 3, -3}, {1, 3, -4}, {2, 1, -3}, {3, 1, -4}, {4, -1, -3}},
    {{-2, -1, 3}, {-1, -1, 2}, {-2, 3, -1}, {-1, 2, -1}, {0, 1, -1}, {1, 2, -3}, {1, 3, -4}, {3, 0, -3}, {4, 0, -4}, {2, -2, 0}, {3, -3, 0}, {4, -4, 0}, {1, -3, 2}, {1, -4, 3}},
    {{-4, -1, 5}, {-2, -3, 5}, {-4, 5, -1}, {-2, 5, -3}, {-1, 3, -2}, {1, 3, -4}, {3, 1, -4}, {4, 2, -6}, {6, 0, -6}, {-1, -2, 3}, {1, -4, 3}, {3, -4, 1}, {4, -6, 2}, {6, -6, 0}},
    {{-1, 0, 1}, {-1, 1, 0}, {-1, -2, 3}, {0, -2, 2}, {-1, 3, -2}, {0, 2, -2}, {1, 3, -4}, {2, 1, -3}, {3, 1, -4}, {3, -1, -2}, {3, -2, -1}, {2, -3, 1}, {3, -4, 1}, {1, -4, 3}},
    {{-1, -1, 2}, {-1, -2, 3}, {-1, 2, -1}, {-1, 3, -2}, {-2, -3, 5}, {-2, 5, -3}, {1, 3, -4}, {1, -4, 3}, {3, -4, 1}, {3, -3, 0}, {4, -6, 2}, {3, 0, -3}, {3, 1, -4}, {4, 2, -6}},
    {{-2, -1, 3}, {-2, 2, 0}, {-1, 2, -1}, {-1, 1, 0}, {1, 2, -3}, {1, 3, -4}, {2, 2, -4}, {0, -1, 1}, {1, -1, 0}, {1, -2, 1}, {3, -4, 1}, {3, -2, -1}, {3, -1, -2}, {4, -2, -2}},
    {{-2, -3, 5}, {0, -3, 3}, {0, -2, 2}, {2, -4, 2}, {2, -3, 1}, {1, -1, 0}, {1, 0, -1}, {0, 2, -2}, {0, 3, -3}, {2, 1, -3}, {2, 2, -4}, {4, 2, -6}, {-2, 5, -3}, {4, -6, 2}},
    {{1, 3, -4}, {1, 2, -3}, {-1, 1, 0}, {3, -1, -2}, {4, -2, -2}, {3, -2, -1}, {2, -2, 0}, {-2, 1, 1}, {-1, 0, 1}, {0, -1, 1}, {0, -3, 3}, {-1, -3, 4}, {2, -4, 2}, {3, -5, 2}},
    {{-2, 0, 2}, {-1, 0, 1}, {-1, 1, 0}, {-1, 3, -2}, {1, 1, -2}, {2, 1, -3}, {3, 1, -4}, {4, -1, -3}, {3, -1, -2}, {3, -2, -1}, {3, -4, 1}, {1, -2, 1}, {0, -2, 2}, {-1, -2, 3}}
};
std::vector<std::tuple<int, int, int>> ObjectCoordinates::deleteCoordinates = {
    {-8, -2, 10}, {-8, -1, 9}, {-8, 0, 8}, {-8, 7, 1}, {-8, 1, 7}, {-8, 8, 0}, 
    {-8, 9, -1}, {-8, 10, -2}, {-7, 0, 7}, {-7, -1, 8}, {-7, -2, 9}, {-7, 7, 0}, 
    {-7, 8, -1}, {-7, 9, -2}, {-7, 10, -3}, {-6, 7, -1}, {-6, 8, -2}, {-6, 9, -3}, 
    {-5, 7, -2}, {-5, 8, -3}, {-5, 9, -4}, {-4, 7, -3}, {-4, 8, -4}, {-3, 7, -4}, 
    {-3, 8, -5}, {-2, 7, -5}, {-1, 7, -6}, {-6, -1, 7}, {-6, -2, 8}, {-6, -3, 9}, 
    {-5, -2, 7}, {-5, -3, 8}, {-4, -3, 7}, {-4, -4, 8}, {-3, -4, 7}, {-2, -5, 7}, 
    {10, -11, 1}, {10, -10, 0}, {10, -9, -1}, {10, -8, -2}, {10, 1, -11}, 
    {10, 0, -10}, {10, -1, -9}, {10, -2, -8}, {9, -1, -8}, {8, 0, -8}, {7, 1, -8}, 
    {6, 2, -8}, {5, 3, -8}, {4, 4, -8}, {5, 4, -9}, {6, 3, -9}, {7, 2, -9}, {7, 3, -10}, 
    {8, 2, -10}, {8, 1, -9}, {9, 0, -9}, {9, 1, -10}, {9, 2, -11}, {9, -8, -1}, {8, -8, 0}, 
    {7, -8, 1}, {6, -8, 2}, {5, -8, 3}, {4, -8, 4}, {6, -9, 3}, {7, -9, 2}, {8, -10, 2}, 
    {8, -9, 1}, {9, -10, 1}, {9, -9, 0}, {3, 5, -8}, {1, 6, -7}
};
std::vector<std::tuple<int, int, int>> ObjectCoordinates::startCoordinates = {
    {10, -7, -3}, {9, -6, -3}, {9, -5, -4}, {9, -4, -5}, {9, -3, -6}, {10, -3, -7}, 
    {10, -4, -6}, {10, -5, -5}, {10, -6, -4}, {-8, 2, 6}, {-7, 2, 5}, {-7, 3, 4}, 
    {-7, 4, 3}, {-7, 5, 2}, {-8, 6, 2}, {-8, 5, 3}, {-8, 4, 4}, {-8, 3, 5}
};

void Board::drawBoard(sf::RenderTarget& target)
{
    for (auto& pair : hexDict) {
        pair.second->draw(target);
    }
}

void Board::deleteHexagons()
{

    for (const auto& coordinates : ObjectCoordinates::getdeleteCoordinates())
    {
        hexDict.erase(coordinates);
    }
}

void Board::setStart()
{

    for (const auto& coordinates : ObjectCoordinates::getStartCoordinates())
    {
        hexDict[coordinates]->setStart(true);
    }
}

void Board::setRocks()
{

    for (const auto& coordinates : ObjectCoordinates::getRockCoordinates())
    {
        hexDict[coordinates] = new Rock(coordinates);
    }
}

void Board::setBases()
{
    std::tuple<int, int, int> base1 = {-4,2,2};
    std::tuple<int, int, int> base2 = { 6,-3,-3};
    
    hexDict[base1]->setBase(true);
    hexDict[base2]->setBase(true);
}

void Board::boardPreperation()
{
    for (int i = 0; i < numRows; i++) {
        for (int j = 0; j < numCols; j++) {
            Grass hexagon;

            float x = j * horizSpacing * hexSize + xOffset;
            float y = i * vertSpacing * hexSize + yOffset + (j % 2) * vertSpacing * hexSize / 2;

            hexagon.setPos(x, y);
            /*std::cout << "x:" << x << " y:" << y << "\n";
            std::cout << "Hx:" << hexagon.x << " Hy:" << hexagon.y << " Hz:" << hexagon.z << "\n";*/

            hexDict[hexagon.getCubeCoords()] = new Grass(hexagon.getCubeCoords());
        }
    }
    deleteHexagons();
    setStart();
    setRocks();
    setBases();
}