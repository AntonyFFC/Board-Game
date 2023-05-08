#pragma once
#include <SFML/Graphics.hpp>
class Hex
{
public:
	int x;
	int y;
	int z;

	bool isRock;
	bool isBase;
	bool isWall;

	float scale;
	float xOffset;
	float yOffset;

	sf::ConvexShape shape;

	Hex(int inx = 0, int iny = 0, int inz = 0, float inxPos = 0.f, float inyPos = 0.f, bool inIsRock = false, bool inIsBase = false, bool inIsWall = false, float inScale = 0.8f, float inXOffset = 30.f, float inYOffset = 20.f);

	void setPos(float inx = 0, float iny = 0);

	void setCoords(int inx = 0, int iny = 0);

	sf::ConvexShape getShape();

	void setScl(float inS = 0.8);

	
private:
	float xPos;
	float yPos;
};