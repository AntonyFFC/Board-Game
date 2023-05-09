#include "Board.h"
#include "Hex.h"
#include <SFML/Graphics.hpp>
//#include <iostream>

Board::Board(int Rows, int Columns, float HSize)
{
    
    // Create the grid of hexagons
    const int numRows = Rows;
    const int numCols = Columns;
    const int numHexagons = numRows * numCols;
    const float hexSize = HSize;
    const float vertSpacing = 86.6f;
    const float horizSpacing = 75.f;
    float xOffset = 30.f;
    float yOffset = 20.f;

    for (int i = 0; i < numRows; i++) {
        for (int j = 0; j < numCols; j++) {
            Hex hexagon;

            float x = j * horizSpacing * hexSize + xOffset;
            float y = i * vertSpacing * hexSize + yOffset + (j % 2) * vertSpacing * hexSize / 2;

            hexagon.setPos(x, y);
            /*std::cout << "x:" << x << " y:" << y << "\n";
            std::cout << "Hx:" << hexagon.x << " Hy:" << hexagon.y << " Hz:" << hexagon.z << "\n";*/

            hexDict[hexagon.cubeCoords] = hexagon;
        }
    }
    
}

class DirectionVectors {
public:
    static std::vector<sf::Vector3i> directionVectors;

    static std::vector<sf::Vector3i> GetDirectionList() {
        return directionVectors;
    }
};

std::vector<sf::Vector3i> DirectionVectors::directionVectors = {
    sf::Vector3i(1, 0, -1), sf::Vector3i(1, -1, 0), sf::Vector3i(0, -1, 1),
    sf::Vector3i(-1, 0, 1), sf::Vector3i(-1, 1, 0), sf::Vector3i(0, 1, -1)
};

std::vector <sf::Vector3i> Board::GetNeighbours(sf::Vector3i hexCoordinates)
{
    if (!hexDict.count(hexCoordinates))
        return std::vector<sf::Vector3i>();

    if (hexNeighboursDict.count(hexCoordinates))
        return hexNeighboursDict[hexCoordinates];

    hexNeighboursDict[hexCoordinates] = std::vector<sf::Vector3i>();
    for (const auto& direction : DirectionVectors::GetDirectionList()) //this adds the neighbours of the new hex to the dictionary
    {
        if (hexDict.count(hexCoordinates + direction))
        {
            hexNeighboursDict[hexCoordinates].push_back(hexCoordinates + direction);
        }
    }

    return hexNeighboursDict[hexCoordinates];
}




