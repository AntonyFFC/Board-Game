#pragma once
#include <SFML/Graphics.hpp>
#include "Hex.h"
#include <map>
#include <tuple>
class Board {
public:
	Board(int Rows = 10, int Columns = 10, float HSize = 1.f);

	/*std::vector<Hex> hexagons;*/

	std::map<std::tuple<int, int, int>, Hex> hexDict; // stores al of the hexes in the dictionary with the coordinates as the index
	std::map<std::tuple<int, int, int>, std::vector<std::tuple<int, int, int>>> hexNeighboursDict; // stores all neighbours of each hex

	std::vector < std::tuple<int, int, int>> GetNeighbours(std::tuple<int, int, int> hexCoordinates);

	void deleteHexagons();
	void setStart();
	void setRocks();
	void setBases();
	void boardPreperation();

};
