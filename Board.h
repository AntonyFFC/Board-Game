#pragma once
#include <SFML/Graphics.hpp>
#include "Hex.h"
#include <map>
class Board {
public:
	Board(int Rows = 10, int Columns = 10, float HSize = 1.f);

	/*std::vector<Hex> hexagons;*/

	std::map<sf::Vector3i, Hex> hexDict; // stores al of the hexes in the dictionary with the coordinates as the index
	std::map<sf::Vector3i, std::vector<sf::Vector3i>> hexNeighboursDict; // stores all neighbours of each hex

	std::vector <sf::Vector3i> GetNeighbours(sf::Vector3i hexCoordinates);
};
