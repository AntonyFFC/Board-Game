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
    xPos = inx;
    yPos = iny;
    x = (xPos - xOffset - midx * 75.f * scale) / (75.f * scale);
    y = -((xPos - xOffset - midx * 75.f * scale) - (yPos - yOffset - midy * 86.6 * scale)*pow(3, 0.5)) / (75.f * scale);
    z = (-(xPos - xOffset - midx * 75.f * scale) - (yPos - yOffset - midy * 86.6 * scale) * pow(3, 0.5)) / (75.f * scale);

    shape.setPosition(xPos, yPos);
}

void Hex::setCoords(int inx, int iny)
{

}

void Hex::setScl(float inS)
{
    shape.setScale(inS, inS);
}