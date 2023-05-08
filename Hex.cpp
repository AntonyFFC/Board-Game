#include "Hex.h"
#include <SFML/Graphics.hpp>
#include <cmath>

Hex::Hex(int inx, int iny, int inz, float inxPos, float inyPos, bool inIsRock, bool inIsBase, bool inIsWall, float inScale, float inXOffset, float inYOffset)
{
	x = inx;
	y = iny;
	z = inz;
	xPos = inxPos;
	yPos = inyPos;
    isRock = inIsRock;
    isBase = inIsBase;
    isWall = inIsWall;
    scale = inScale;
    xOffset = inXOffset;
    yOffset = inYOffset;

    shape = getShape();
    if (x || y) {
        setCoords(x, y);
    }
    else if (xPos || yPos) {
        setPos(xPos, yPos);
    }

    setScl(scale);
}

sf::ConvexShape Hex::getShape()
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

void Hex::setPos(float inx, float iny)
{
    int midx = 10;
    int midy = 6;
    const float vertSpacing = 86.6f;
    const float horizSpacing = 75.f;
    xPos = inx;
    yPos = iny;
    x = std::round((xPos - xOffset - midx * horizSpacing * scale) / (horizSpacing * scale));
    y = std::round(((xOffset + midx * horizSpacing * scale - xPos) - (yOffset + midy * vertSpacing * scale - yPos) * sqrt(3)) / (2 * horizSpacing * scale));
    z = std::round(((yOffset + midy * vertSpacing * scale - yPos) * sqrt(3) - (xPos - xOffset - midx * horizSpacing * scale)) / (2 * horizSpacing * scale));

    shape.setPosition(xPos, yPos);
}

void Hex::setCoords(int inx, int iny)
{

}

void Hex::setScl(float inS)
{
    shape.setScale(inS, inS);
}