#include "Hex.h"
#include <SFML/Graphics.hpp>
#include <cmath>
#include <SFML/System.hpp>

Hex::Hex(std::tuple<int, int, int> inCoords, float inxPos, float inyPos)
{
    cubeCoords = inCoords;
    xPos = inxPos;
    yPos = inyPos;
    highlight = false;
    scale = 0.8f;
    xOffset = 30.f;
    yOffset = 20.f;
    pawn = nullptr;
    startColor = sf::Color(134, 179, 0);
    base = sf::Color(104, 176, 163);
    startHigh = sf::Color(185, 224, 67);
    baseHigh = sf::Color(159, 227, 215);
    shape = initShape();
    if (std::get<0>(cubeCoords)!=-99 && std::get<1>(cubeCoords)!=-99 && std::get<2>(cubeCoords)!=-99) {
        setCoords(std::get<0>(cubeCoords), std::get<1>(cubeCoords), std::get<2>(cubeCoords));
    }
    else if (xPos || yPos) {
        setPos(xPos, yPos);
    }
    setScl(scale);
}

Hex::~Hex() {
   delete pawn;
}

sf::ConvexShape Hex::initShape() const
{

    sf::ConvexShape hexTile(6);
    hexTile.setPoint(0, sf::Vector2f(0, 0));
    hexTile.setPoint(1, sf::Vector2f(50, 0));
    hexTile.setPoint(2, sf::Vector2f(75, 43.3));
    hexTile.setPoint(3, sf::Vector2f(50, 86.6));
    hexTile.setPoint(4, sf::Vector2f(0, 86.6));
    hexTile.setPoint(5, sf::Vector2f(-25, 43.3));
    hexTile.setPosition(sf::Vector2f(100, 100));
    hexTile.setOutlineColor(out);
    hexTile.setOutlineThickness(2.5f);
    hexTile.setFillColor(fill);
    return hexTile;
}

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
}

void Hex::setCoords(int inx, int iny, int inz)
{
    int midx = 8;
    int midy = 6;
    const float vertSpacing = 86.6f;
    const float horizSpacing = 75.f;
    cubeCoords = { inx, iny, inz };
    xPos = std::get<0>(cubeCoords) * (horizSpacing * scale) + midx * horizSpacing * scale + xOffset;
    yPos = (std::get<1>(cubeCoords) * (2 * horizSpacing * scale) - (xOffset + midx * horizSpacing * scale - xPos)) / sqrt(3) + yOffset + midy * vertSpacing * scale;

    shape.setPosition(xPos, yPos);
}

void Hex::setScl(float inS)
{
    shape.setScale(inS, inS);
}

float Hex::getRadius() const
{
    const float vertSpacing = 86.6f;
    return scale * vertSpacing / 2;
}

sf::Vector2f Hex::getOrigin() const
{
    sf::Vector2f middlePoint(0, 0);
    for (unsigned int i = 0; i < shape.getPointCount(); i++) {
        middlePoint += shape.getPoint(i);
    }
    middlePoint /= static_cast<float>(shape.getPointCount());

    return shape.getTransform().transformPoint(middlePoint);
}

void Hex::setRock(bool boolean)
{
    isRock_ = boolean;
}

void Hex::setWall(bool boolean)
{
    isWall_ = boolean;
}

void Hex::setBase(bool boolean)
{
    if (boolean) {
        currentFill = base;
        currentOut = base;
        currentHighFill = baseHigh;
    }
    setColour();
    isBase_ = boolean;
}

void Hex::setGrass(bool boolean)
{
    isGrass_ = boolean;
}

void Hex::setStart(bool boolean)
{
    if (boolean) {
        currentFill = startColor;
        currentHighFill = startHigh;
    }
    setColour();
    isStart_ = boolean;

}

void Hex::clearHex(bool boolean) {
    if (boolean) {
        currentFill = fill;
        currentOut = out;
        currentHighFill = highFill;
        setStart(isStart());
        setBase(isBase());
    }
    setColour();
}

void Hex::setPawn(bool boolean)
{
    if (boolean)
        pawn = new Pawn();
    else
        delete pawn;
}

void Hex::setColour()
{
    shape.setFillColor(currentFill);
    shape.setOutlineColor(currentOut);
}

bool Hex::isRock() const {
    return isRock_;
}

bool Hex::isWall() const {
    return isWall_;
}

bool Hex::isBase() const {
    return isBase_;
}

bool Hex::isGrass() const {
    return isGrass_;
}

bool Hex::isStart() const {
    return isStart_;
}

bool Hex::isPawn() const {
    return isPawn_;
}

std::tuple<int, int, int> Hex::getCubeCoords() {
    return cubeCoords;
}

sf::ConvexShape Hex::getShape() const {
    return shape;
}

float Hex::getXpos() const {
    return xPos;
}

float Hex::getYpos() const {
    return yPos;
}

Grass::Grass(std::tuple<int, int, int> inCoords, float inxPos, float inyPos)
    :Hex(inCoords, inxPos, inyPos)
{
    fill = sf::Color(37, 142, 37);
    out = sf::Color(40, 66, 42);
    highFill = sf::Color(89, 207, 89);
    highOut = sf::Color(225, 245, 5);
    setGrass(true);
    clearHex(true);
}

void Grass :: setHighlight(bool boolean)
{
    if (boolean)
    {
        currentFill = currentHighFill;
        currentOut = highOut;
    }
    clearHex(!boolean);
    highlight = boolean;
}


Rock::Rock(std::tuple<int, int, int> inCoords, float inxPos, float inyPos)
    :Hex(inCoords, inxPos, inyPos)
{
    fill = sf::Color(115, 115, 115);
    out = sf::Color(0, 0, 0);
    setRock(true);
    clearHex(true);
}

void Rock::setHighlight(bool boolean)
{
    return;
}

Wall::Wall(std::tuple<int, int, int> inCoords, float inxPos, float inyPos)
    :Hex(inCoords, inxPos, inyPos)
{    
    fill = sf::Color(140, 62, 34);
    out = sf::Color(71, 31, 16);
    highFill = sf::Color(36, 8, 5);
    highOut = sf::Color(225, 245, 5);
    setWall(true);
    clearHex(true);
}

void Wall::setHighlight(bool boolean)
{
    if (boolean)
    {
        currentFill = currentHighFill;
        currentOut = highOut;
    }
    clearHex(!boolean);
    highlight = boolean;
}