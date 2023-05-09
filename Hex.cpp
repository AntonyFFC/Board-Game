#include "Hex.h"
#include <SFML/Graphics.hpp>
#include <cmath>
#include <SFML/System.hpp>

Hex::Hex(std::tuple<int, int, int> inCoords, float inxPos, float inyPos, bool inIsRock, bool inIsBase, bool inIsWall,
    float inScale, float inXOffset, float inYOffset, sf::Color inNormalFill, sf::Color inNormalOut, sf::Color inHighFill, sf::Color inHighOut)
{
    cubeCoords = inCoords;
    /*x = inx;
    y = iny;
    z = inz;*/
    xPos = inxPos;
    yPos = inyPos;
    isRock = inIsRock;
    isBase = inIsBase;
    isWall = inIsWall;
    scale = inScale;
    xOffset = inXOffset;
    yOffset = inYOffset;

    normalFill = inNormalFill;
    normalOut = inNormalOut;
    highFill = inHighFill;
    highOut = inHighOut;

    shape = getShape();
    if (std::get<0>(cubeCoords) || std::get<1>(cubeCoords) || std::get<2>(cubeCoords)) {
        setCoords(std::get<0>(cubeCoords), std::get<1>(cubeCoords), std::get<2>(cubeCoords));
    }
    else if (xPos || yPos) {
        setPos(xPos, yPos);
    }

    setScl(scale);
}

sf::ConvexShape Hex::getShape()
{

    sf::ConvexShape hexTile(6);
    hexTile.setPoint(0, sf::Vector2f(0, 0));
    hexTile.setPoint(1, sf::Vector2f(50, 0));
    hexTile.setPoint(2, sf::Vector2f(75, 43.3));
    hexTile.setPoint(3, sf::Vector2f(50, 86.6));
    hexTile.setPoint(4, sf::Vector2f(0, 86.6));
    hexTile.setPoint(5, sf::Vector2f(-25, 43.3));
    hexTile.setPosition(sf::Vector2f(100, 100));
    hexTile.setOutlineColor(normalOut);
    hexTile.setOutlineThickness(2.5f);
    hexTile.setFillColor(normalFill);
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
    std::get<0>(cubeCoords) = std::round((xPos - xOffset - midx * horizSpacing * scale) / (horizSpacing * scale));
    std::get<1>(cubeCoords) = std::round(((xOffset + midx * horizSpacing * scale - xPos) - (yOffset + midy * vertSpacing * scale - yPos) * sqrt(3)) / (2 * horizSpacing * scale));
    std::get<2>(cubeCoords) = std::round(((yOffset + midy * vertSpacing * scale - yPos) * sqrt(3) - (xPos - xOffset - midx * horizSpacing * scale)) / (2 * horizSpacing * scale));

    shape.setPosition(xPos, yPos);
}

void Hex::setCoords(int inx, int iny, int inz)
{
    //TO DO
}

void Hex::setScl(float inS)
{
    shape.setScale(inS, inS);
}

void Hex::disableHighlight()
{
    highlight = false;
    shape.setFillColor(normalFill);
    shape.setOutlineColor(normalOut);
}

void Hex::enableHighlight()
{
    highlight = true;
    shape.setFillColor(highFill);
    shape.setOutlineColor(highOut);
}

float Hex::getRadius()
{
    const float vertSpacing = 86.6f;
    return scale * vertSpacing / 2;
}

sf::Vector2f Hex::getOrigin()
{
    // Get the local position of the middle point (i.e., the average of all vertices)
    sf::Vector2f middlePoint(0, 0);
    for (unsigned int i = 0; i < shape.getPointCount(); i++) {
        middlePoint += shape.getPoint(i);
    }
    middlePoint /= static_cast<float>(shape.getPointCount());

    // Apply the current transformation matrix to obtain the global position
    return shape.getTransform().transformPoint(middlePoint);
}