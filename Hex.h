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
	// setters
	void setRock(bool boolean);
	void setWall(bool boolean);
	void setBase(bool boolean);
	void setGrass(bool boolean);
	void setStart(bool boolean);
	void setPawn(bool boolean, Pawn* inPawn = nullptr);
	void setBody(bool boolean, Pawn* inBody = nullptr);
	void setBlocking(bool boolean);
	void setPos(float inx = 0, float iny = 0);
	void setCoords(int inx = 0, int iny = 0, int inz = 0);
	void setPawnDist(int dist);

	//getters
	sf::Vector2f getOrigin() const;
	std::tuple<int, int, int> getCubeCoords();
	virtual sf::ConvexShape getShape() = 0;
	sf::Vector2f getPos() const;
	int getPawnDist() const;

	// bools
	bool isRock() const;
	bool isWall() const;
	bool isBase() const;
	bool isGrass() const;
	bool isStart() const;
	bool isPawn() const;
	bool hasBody() const;
	bool isBlocking() const;
	bool isHigh(int col) const;
	bool isClicked(sf::Vector2i mousePosition) const;

	virtual void setHighlight(bool boolean, int col);
	void draw(sf::RenderTarget& target);
	void setScl(float inS = 0.8);	
	Pawn* pawn;
	Pawn* body;

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
	int pawnDist_;
	bool isRock_ = false;
	bool isWall_ = false;
	bool isBase_ = false;
	bool isGrass_ = false;
	bool isStart_ = false;
	bool isPawn_ = false;
	bool hasBody_ = false;
	bool highlights[3];
	bool isBlocking_ = false;
	sf::Color fill;
	sf::Color out;
	std::vector<sf::Color> highFill;
	std::vector<sf::Color> highOut;
	sf::Color startColor;
	sf::Color base;
	std::vector<sf::Color> startHigh;
	std::vector<sf::Color> baseHigh;

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
	void setHighlight(bool boolean, int col) override;
	sf::ConvexShape getShape() override;
};

class Wall : public Hex
{
public:
	Wall(std::tuple<int, int, int> inCoords = { 0, 0, 0 }, float inxPos = 0.f, float inyPos = 0.f);
	sf::ConvexShape getShape() override;
};