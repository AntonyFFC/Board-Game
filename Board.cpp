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




