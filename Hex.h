#pragma once
#include <SFML/Graphics.hpp>
#include <tuple>
class Hex
{
public:
	std::tuple<int, int, int> cubeCoords;
	/*int x;
	int y;
	int z;*/

	bool isRock;
	bool isBase;
	bool isWall;
	bool highlight;

	float scale;
	float xOffset;
	float yOffset;

	sf::Color normalFill;
	sf::Color normalOut;
	sf::Color highFill;
	sf::Color highOut;

	sf::ConvexShape shape;

	Hex(std::tuple<int, int, int> inCoords = { 0,0,0 }, float inxPos = 0.f, float inyPos = 0.f, bool inIsRock = false, bool inIsBase = false, bool inIsWall = false,
		float inScale = 0.8f, float inXOffset = 30.f, float inYOffset = 20.f, sf::Color inNormalFill = sf::Color(37, 142, 37), sf::Color inNormalOut = sf::Color(165, 42, 42),
		sf::Color inHighFill = sf::Color(102, 255, 153), sf::Color inHighOut = sf::Color(225, 245, 5));

	void setPos(float inx = 0, float iny = 0);

	void setCoords(int inx = 0, int iny = 0, int inz = 0);

	sf::ConvexShape getShape();

	void setScl(float inS = 0.8);

	void disableHighlight();

	void enableHighlight();

	float getRadius();

	sf::Vector2f getOrigin();


private:
	float xPos;
	float yPos;
};