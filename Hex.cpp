#include "Hex.h"
#include <SFML/Graphics.hpp>

Hex::Hex(int inx, int iny, int inz, float inxPos, float inyPos, bool inIsRock, bool inIsBase, bool inIsWall)
{
	x = inx;
	y = iny;
	z = inz;
	xPos = inxPos;
	yPos = inyPos;
    isRock = inIsRock;
    isBase = inIsBase;
    isWall = inIsWall;
}

sf::ConvexShape Hex::getHexShape()
{
    sf::Color brown(165, 42, 42);
    sf::Color green(37, 142, 37);

    sf::ConvexShape hexTile(6);
    hexTile.setPoint(0, sf::Vector2f(0, 0));
    hexTile.setPoint(1, sf::Vector2f(50, 0));
    hexTile.setPoint(2, sf::Vector2f(75, 43.3));
    hexTile.setPoint(3, sf::Vector2f(50, 86.6));
    hexTile.setPoint(4, sf::Vector2f(0, 86.6));
    hexTile.setPoint(5, sf::Vector2f(-25, 43.3));
    hexTile.setPosition(sf::Vector2f(100, 100));
    hexTile.setOutlineColor(brown);
    hexTile.setOutlineThickness(2.5f);
    hexTile.setFillColor(green);
    return hexTile;
}