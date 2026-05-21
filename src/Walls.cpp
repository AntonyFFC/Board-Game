#include "Walls.h"

Walls::Walls(sf::RenderWindow* window)
	: target(window)
{
	numberOfWalls[0] = 0;
	numberOfWalls[1] = 0;
}

Walls::~Walls() = default;

void Walls::draw()
{
	float radius = 30.f;
	sf::CircleShape circle(radius);
	circle.setFillColor(sf::Color(139, 69, 19)); // brown
	sf::Vector2u targetSize = target->getSize();
	for (int team = 0; team < 2; team++) {
		sf::Vector2f pos(targetSize.x * 0.02+1100*((team+1)%2), targetSize.y * 0.82);
		for (int i = 0; i < numberOfWalls[team]; i++) {
			circle.setPosition(pos);
			target->draw(circle);
			if (team == 0)
				pos -= sf::Vector2f(radius * 2.5, 0);
			else
				pos += sf::Vector2f(radius * 2.5, 0);
		}
	}
}

void Walls::addWall(int teamNum)
{
	numberOfWalls[teamNum]++;
}

void Walls::addWalls(int numWalls, int teamNum)
{
	numberOfWalls[teamNum] += numWalls;
}

bool Walls::buildWall(int teamNum)
{
	if (numberOfWalls[teamNum] > 0)
	{
		numberOfWalls[teamNum]--;
		return true;
	}
	return false;
}
