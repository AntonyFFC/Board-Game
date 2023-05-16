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
    highlight = false;
    scale = inScale;
    xOffset = inXOffset;
    yOffset = inYOffset;

    normalFill = inNormalFill;
    normalOut = inNormalOut;
    highFill = inHighFill;
    highOut = inHighOut;

    shape = getShape();
    /*rockShape = getRockShape();*/

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

//sf::ConvexShape Hex::getRockShape()
//{
//
//    sf::ConvexShape rockTile(7);
//    rockTile.setPoint(0, sf::Vector2f(11, 13.7));
//    rockTile.setPoint(1, sf::Vector2f(53, 30));
//    rockTile.setPoint(2, sf::Vector2f(50.8, 53.5));
//    rockTile.setPoint(3, sf::Vector2f(24.5, 72.5));
//    rockTile.setPoint(4, sf::Vector2f(-5.3, 62));
//    rockTile.setPoint(5, sf::Vector2f(-3.6, 29.4));
//    rockTile.setPoint(6, sf::Vector2f(18.4, 28.3));
//    rockTile.setPosition(shape.getPosition());
//    rockTile.setOutlineColor(normalFill);
//    rockTile.setOutlineThickness(2.5f);
//    rockTile.setFillColor(normalFill);
//    return rockTile;
//}

void Hex::setPos(float inx, float iny)
{
    int midx = 8;
    int midy = 6;
    const float vertSpacing = 86.6f;
    const float horizSpacing = 75.f;
    xPos = inx;
    yPos = iny;
    std::get<0>(cubeCoords) = std::round((xPos - xOffset - midx * horizSpacing * scale) / (horizSpacing * scale));
    std::get<1>(cubeCoords) = std::round(((xOffset + midx * horizSpacing * scale - xPos) - (yOffset + midy * vertSpacing * scale - yPos) * sqrt(3)) / (2 * horizSpacing * scale));
    std::get<2>(cubeCoords) = std::round(((yOffset + midy * vertSpacing * scale - yPos) * sqrt(3) - (xPos - xOffset - midx * horizSpacing * scale)) / (2 * horizSpacing * scale));

    shape.setPosition(xPos, yPos);
    /*rockShape.setPosition(xPos, yPos);*/
}

void Hex::setCoords(int inx, int iny, int inz)
{
    int midx = 8;
    int midy = 6;
    const float vertSpacing = 86.6f;
    const float horizSpacing = 75.f;
    cubeCoords = { inx, iny, inz };
    xPos = std::get<0>(cubeCoords) * (horizSpacing * scale) + midx * horizSpacing * scale + xOffset;
    yPos = (std::get<1>(cubeCoords) * (2 * horizSpacing * scale) - (xOffset + midx * horizSpacing * scale - xPos)) / sqrt(3) - yOffset + midy * vertSpacing * scale;

    shape.setPosition(xPos, yPos);
    /*rockShape.setPosition(xPos, yPos);*/
}

void Hex::setScl(float inS)
{
    shape.setScale(inS, inS);
    /*rockShape.setScale(inS, inS);*/
}

void Hex::disableHighlight()
{
    if (!highlight)
        return;
    highlight = false;
    shape.setFillColor(normalFill);
    shape.setOutlineColor(normalOut);
    if (isRock)
        setRock();
    else if (isWall)
        setWall();
    else if (isStart)
        setStart();
    else if (isBase)
        setBase();
}

void Hex::enableHighlight()
{
    if (highlight)
        return;
    highlight = true;
    sf::Color Fill = highFill;
    sf::Color Out = highOut;

    if (isRock || isWall)
    {
        return;
        /*Fill = sf::Color(90, 143, 93);
        Out = sf::Color(22, 36, 23);*/
    }
    else if(isStart)
    {
        Fill = sf::Color(185, 224, 67);
        Out = highOut;
    }
    else if (isBase)
    {
        Fill = sf::Color(159, 227, 215);
        Out = highOut;
    }
    shape.setFillColor(Fill);
    shape.setOutlineColor(Out);
}

float Hex::getRadius()
{
    const float vertSpacing = 86.6f;
    return scale * vertSpacing / 2;
}

sf::Vector2f Hex::getOrigin()
{
    sf::Vector2f middlePoint(0, 0);
    for (unsigned int i = 0; i < shape.getPointCount(); i++) {
        middlePoint += shape.getPoint(i);
    }
    middlePoint /= static_cast<float>(shape.getPointCount());

    return shape.getTransform().transformPoint(middlePoint);
}

void Hex::setRock()
{
    sf::Color rockFill = sf::Color(115, 115, 115);
    sf::Color rockOut = sf::Color(0, 0, 0);
    isRock = true;
    shape.setFillColor(rockFill);
    shape.setOutlineColor(rockOut);

}

void Hex::setWall()
{
    sf::Color wallFill = sf::Color(140, 62, 34);
    sf::Color wallOut = sf::Color(71, 31, 16);
    isWall = true;
    shape.setFillColor(wallFill);
    shape.setOutlineColor(wallOut);

}

void Hex::setBase()
{
    sf::Color baseFill = sf::Color(104, 176, 163);
    sf::Color baseOut = sf::Color(104, 176, 163);
    isBase = true;
    shape.setFillColor(baseFill);
    shape.setOutlineColor(baseOut);

}

void Hex::setGrass()
{
    isRock = false;
    isWall = false;
    isBase = false;
    isStart = false;
    shape.setOutlineColor(normalOut);
    shape.setOutlineThickness(2.5f);
    shape.setFillColor(normalFill);
}

void Hex::setStart()
{
    sf::Color startColour = sf::Color(134, 179, 0);
    isStart = true;
    shape.setFillColor(startColour);
    shape.setOutlineColor(normalOut);

}