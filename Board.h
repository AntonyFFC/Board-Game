#pragma once
#include <SFML/Graphics.hpp>
class Board {
public:
	Board();
	std::vector<sf::ConvexShape> hexagons;
};
