#pragma once
#include <SFML/Graphics.hpp>
#include "Hex.h"
#include <map>
#include <tuple>
#include "Pawn.h"
#include <algorithm>
#include "Equipment.h"
class Board {
public:
	Board(int Rows = 10, int Columns = 10, float HSize = 1.f);
	~Board();

	/*std::vector<Hex> hexagons;*/

	std::map<std::tuple<int, int, int>, Hex*> hexDict; // stores al of the hexes in the dictionary with the coordinates as the index
	std::map<std::tuple<int, int, int>, std::vector<std::tuple<int, int, int>>> hexNeighboursDict; // stores all neighbours of each hex

	std::vector < std::tuple<int, int, int>> getNeighbours(std::tuple<int, int, int> hexCoordinates);
	std::vector < std::tuple<int, int, int>> getInRange(std::tuple<int, int, int> hexCoordinates, int dist = 1, int minDistance = 1);
	std::vector < std::tuple<int, int, int>> getReachable(std::tuple<int, int, int> start, int movement = 5);

	void drawBoard(sf::RenderTarget& target);
	void deleteHexagons();
	void setStart();
	void setRocks();
	void setBases();
	void boardPreperation();
	void handleClick(sf::Vector2i mousePosition);
private:
	int numRows;
	int numCols;
	float hexSize;
	int numHexagons;
	float vertSpacing;
	float horizSpacing;
	float xOffset;
	float yOffset;
	std::vector< std::tuple<int, int, int>> highlighted = std::vector< std::tuple<int, int, int>>();
	std::tuple<int, int, int> previous = std::tuple<int, int, int>();
};
