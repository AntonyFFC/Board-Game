#pragma once
#include "Hex.h"
#include "Board.h"
#include <SFML/Graphics.hpp>
#include "Pawn.h"

class selectionManager {
private:
	std::vector< std::tuple<int, int, int>> neighbours = std::vector< std::tuple<int, int, int>>();
public:
	void handleClick(Board& grid, sf::Vector2i mousePosition);
};

