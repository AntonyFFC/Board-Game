#include "Hex.h"
#include <SFML/Graphics.hpp>
#include <cmath>
#include <SFML/System.hpp>

Hex::Hex(std::tuple<int, int, int> inCoords, float inxPos, float inyPos)
{
    cubeCoords = inCoords;
    xPos = inxPos;
    yPos = inyPos;
    for (int i = 0; i < sizeof(highlights); i++)
    {
        highlights[i] = false;
    }
    scale = 0.8f;
	whosPawn = 0;
    xOffset = 30.f;
    yOffset = 20.f;
    setPawn(false);
    setBody(false);
    startColor = sf::Color(134, 179, 0);
    base = sf::Color(104, 176, 163);
    highFillRed = { sf::Color(89, 207, 89), sf::Color(207, 89, 89), sf::Color(156,140,89) };
    highFillBlue = { sf::Color(89, 207, 89), sf::Color(87, 91, 207), sf::Color(156,140,89) };
    highOut = { sf::Color(225, 245, 5), sf::Color(225, 245, 5), sf::Color(225, 245, 5) };
    shape = initShape();
    setCoords(std::get<0>(cubeCoords), std::get<1>(cubeCoords), std::get<2>(cubeCoords));
    setScl(scale);
}

Hex::~Hex() {
   delete pawn;
   delete body;
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
    scale = inS;
    shape.setScale(scale, scale);
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
    isBase_ = boolean;
}

void Hex::setGrass(bool boolean)
{
    isGrass_ = boolean;
}

void Hex::setStart(bool boolean)
{
    isStart_ = boolean;
}

void Hex::setPawn(bool boolean, Pawn* inPawn)
{
    if (boolean)
    {
        pawn = inPawn;
        pawn->setHexCoords(cubeCoords);
    }
    else if (pawn != nullptr) {
        pawn = nullptr;
    }
    isPawn_ = boolean;
}

void Hex::setBody(bool boolean, Pawn* inBody)
{
    if (boolean)
    {
        body = inBody;
        body->setHexCoords(cubeCoords);
    }
    else if (body != nullptr) {
        body = nullptr;
    }
    hasBody_ = boolean;
}

void Hex::setEquipmentPile(bool boolean, EquipmentPile* inPile)
{
	if (boolean)
	{
		equipmentPile = inPile;
	}
	else if (equipmentPile != nullptr) {
		equipmentPile = nullptr;
	}
	hasEquipmentPile_ = boolean;
}

void Hex::setBlocking(bool boolean)
{
    isBlocking_ = boolean;
}

void Hex::setColour(sf::Color inFill, sf::Color inOut)
{
    shape.setFillColor(inFill);
    shape.setOutlineColor(inOut);
}

void Hex::setPawnDist(int dist)
{
    pawnDist_ = dist;
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

bool Hex::isBlocking() const {
    return isBlocking_;
}

bool Hex::isBlockingForPawn(Pawn* inPawn)
{
    if (!isPawn()) return isBlocking_;
    return isBlocking_ || !(inPawn->getTeamNumber() == pawn->getTeamNumber() && 
        inPawn->getSide() == pawn->getSide());
}

bool Hex::isHigh(int col) const {
    return highlights[col];
}

bool Hex::hasBody() const
{
    return hasBody_;
}

std::tuple<int, int, int> Hex::getCubeCoords() {
    return cubeCoords;
}

sf::Vector2f Hex::getPos() const {
    return sf::Vector2f(xPos,yPos);
}

int Hex::getPawnDist() const
{
    return pawnDist_;
}

void Hex::setHighlight(int col, int inWhosPawn)
{
	this->whosPawn = inWhosPawn;
    highlights[col] = true;
}

void Hex::clearHighlight(int col)
{
	highlights[col] = false;
}

bool Hex::isClicked(sf::Vector2i mousePosition) const {
    const float hexRadius = getRadius();
    sf::Vector2f hexOrigin = getOrigin();
    float distance = std::sqrt(
        std::pow(mousePosition.x - hexOrigin.x, 2) +
        std::pow(mousePosition.y - hexOrigin.y, 2)
    );
    return distance < hexRadius;
}

bool Hex::hasEquipmentPile() const
{
	return hasEquipmentPile_;
}

void Hex::draw(sf::RenderTarget& target) {
    target.draw(getShape());
}

Grass::Grass(std::tuple<int, int, int> inCoords, float inxPos, float inyPos)
    :Hex(inCoords, inxPos, inyPos)
{
    fill = sf::Color(37, 142, 37);
    out = sf::Color(40, 66, 42);
    setGrass(true);
}

sf::Color adjustColor(const sf::Color& color, int adjustment) {
    sf::Uint8 r = std::min(255, color.r + adjustment);
    sf::Uint8 g = std::min(255, color.g + adjustment);
    sf::Uint8 b = std::min(255, color.b + adjustment);
    return sf::Color(r, g, b, color.a); // Preserve the alpha channel
}

sf::ConvexShape Grass::getShape() {
    sf::Color currentFill = fill;
    sf::Color currentOut = out;

    int largestHighIndex = -1;
    int arrSize = sizeof(highlights) / sizeof(highlights[0]);
    for (int i = 0; i < arrSize; i++) {
        if (highlights[i]) {
            largestHighIndex = i;
        }
    }

    if (largestHighIndex == -1) {
        if (isStart())
        {
            currentFill = startColor;
		}
		else if (isBase())
		{
			currentFill = base;
		}
    }
    else
    {
		if (whosPawn == 0)
		{
			currentFill = highFillRed[largestHighIndex];
			currentOut = highOut[largestHighIndex];
		}
		else
		{
			currentFill = highFillBlue[largestHighIndex];
			currentOut = highOut[largestHighIndex];
		}

        if (isStart() || isBase())
        {
            currentFill = adjustColor(currentFill, 30);
        }
    }
    setColour(currentFill, currentOut);
    return shape;
}

Rock::Rock(std::tuple<int, int, int> inCoords, float inxPos, float inyPos)
    :Hex(inCoords, inxPos, inyPos)
{
    fill = sf::Color(115, 115, 115);
    out = sf::Color(0, 0, 0);
    setRock(true);
    setBlocking(true);
}

void Rock::setHighlight(int col, int inWhosPawn)
{
    return;
}

sf::ConvexShape Rock::getShape() {
    sf::Color currentFill = fill;
    sf::Color currentOut = out;
    setColour(currentFill, currentOut);
    return shape;
}

Wall::Wall(std::tuple<int, int, int> inCoords, float inxPos, float inyPos)
    :Hex(inCoords, inxPos, inyPos)
{    
    fill = sf::Color(140, 62, 34);
    out = sf::Color(71, 31, 16);
    highFillRed = { sf::Color(36, 8, 5)};
    setWall(true);
    setBlocking(true);
}

sf::ConvexShape Wall::getShape() {
    sf::Color currentFill = fill;
    sf::Color currentOut = out;
    if (isHigh(0) || isHigh(1) || isHigh(2))
    {
        currentFill = highFillRed[0];
        currentOut = highOut[0];
    }
    setColour(currentFill, currentOut);
    return shape;
}