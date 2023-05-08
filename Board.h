#pragma once
#include <SFML/Graphics.hpp>
#include "Hex.h"
class Board {
public:
	Board(int Rows = 10, int Columns = 10, float HSize = 1.f);
	std::vector<Hex> hexagons;
};
