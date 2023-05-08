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

            hexagons.push_back(hexagon);
        }
    }
    
}