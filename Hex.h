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

	Hex(int inx = 0, int iny = 0, int inz = 0, float inxPos = 0.f, float inyPos = 0.f, bool inIsRock = false, bool inIsBase = false, bool inIsWall = false);

	sf::ConvexShape getHexShape();

	
private:
	float xPos;
	float yPos;
};