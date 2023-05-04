#include "Board.h"
#include "Hex.h"
#include <SFML/Graphics.hpp>

Board::Board() 
{
    Hex hexagon;
    // Create the grid of hexagons
    const int numRows = 13;
    const int numCols = 21;
    const int numHexagons = numRows * numCols;
    const float hexSize = 0.8f;
    const float vertSpacing = 86.6f;
    const float horizSpacing = 75.f;
    float xOffset = 30.f;
    float yOffset = 20.f;

    for (int i = 0; i < numRows; i++) {
        for (int j = 0; j < numCols; j++) {
            sf::ConvexShape hex = hexagon.getHexShape();
            float x = j * horizSpacing * hexSize + xOffset;
            float y = i * vertSpacing * hexSize + yOffset + (j % 2) * vertSpacing * hexSize / 2;
            hex.setScale(hexSize, hexSize);
            hex.setPosition(x, y);

            hexagons.push_back(hex);
        }
    }
}