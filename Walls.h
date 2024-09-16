#pragma once
#include <SFML/Graphics.hpp>
class Walls
{
public:
	Walls(sf::RenderWindow* window);
	~Walls();
	void draw();
	void addWall(int teamNum);
	void addWalls(int numWalls, int teamNum);
	bool buildWall(int teamNum);

private:
	sf::RenderWindow* target;
	int numberOfWalls[2];
};

