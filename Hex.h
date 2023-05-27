#pragma once
#include <SFML/Graphics.hpp>
#include <tuple>
#include "Pawn.h"


class Hex
{
public:
	friend class Grass;
	friend class Rock;
	friend class Wall;

	Hex(std::tuple<int, int, int> inCoords = { -99, -99, -99 }, float inxPos = 0.f, float inyPos = 0.f);
	~Hex();
	void setRock(bool boolean);
	void setWall(bool boolean);
	void setBase(bool boolean);
	void setGrass(bool boolean);
	void setStart(bool boolean);
	void setPawn(bool boolean, Pawn* inPawn = nullptr);
	void setPos(float inx = 0, float iny = 0);
	void setCoords(int inx = 0, int iny = 0, int inz = 0);
	bool isRock() const;
	bool isWall() const;
	bool isBase() const;
	bool isGrass() const;
	bool isStart() const;
	bool isPawn() const;
	bool isHigh() const;
	sf::Vector2f getOrigin() const;
	virtual void setHighlight(bool boolean);
	std::tuple<int, int, int> getCubeCoords();
	virtual sf::ConvexShape getShape() = 0;
	sf::Vector2f getPos() const;
	bool isClicked(sf::Vector2i mousePosition) const;
	void draw(sf::RenderTarget& target);
	void setScl(float inS = 0.8);	

private:
	sf::ConvexShape initShape() const;
	float getRadius() const;
	void setColour(sf::Color inFill, sf::Color inOut);

	sf::ConvexShape shape;
	std::tuple<int, int, int> cubeCoords;
	float xPos;
	float yPos;
	float scale;
	float xOffset;
	float yOffset;
	bool isRock_ = false;
	bool isWall_ = false;
	bool isBase_ = false;
	bool isGrass_ = false;
	bool isStart_ = false;
	bool isPawn_ = false;
	Pawn* pawn;
	bool highlight;
	sf::Color fill;
	sf::Color out;
	sf::Color highFill;
	sf::Color highOut;
	sf::Color startColor;
	sf::Color base;
	sf::Color startHigh;
	sf::Color baseHigh;

};

class Grass : public Hex
{
public:
	Grass(std::tuple<int, int, int> inCoords = { 0, 0, 0 }, float inxPos = 0.f, float inyPos = 0.f);
	sf::ConvexShape getShape() override;

};

class Rock : public Hex
{
public:
	Rock(std::tuple<int, int, int> inCoords = { 0, 0, 0 }, float inxPos = 0.f, float inyPos = 0.f);
	void setHighlight(bool boolean) override;
	sf::ConvexShape getShape() override;
};

class Wall : public Hex
{
public:
	Wall(std::tuple<int, int, int> inCoords = { 0, 0, 0 }, float inxPos = 0.f, float inyPos = 0.f);
	sf::ConvexShape getShape() override;
};