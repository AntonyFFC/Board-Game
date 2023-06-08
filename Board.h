#pragma once
#include <SFML/Graphics.hpp>
#include "Hex.h"
#include <map>
#include <tuple>
#include "Pawn.h"
class Board {
public:
	Board(int Rows = 10, int Columns = 10, float HSize = 1.f);

	/*std::vector<Hex> hexagons;*/

	std::map<std::tuple<int, int, int>, Hex*> hexDict; // stores al of the hexes in the dictionary with the coordinates as the index
	std::map<std::tuple<int, int, int>, std::vector<std::tuple<int, int, int>>> hexNeighboursDict; // stores all neighbours of each hex

	std::vector < std::tuple<int, int, int>> GetNeighbours(std::tuple<int, int, int> hexCoordinates);
	std::vector < std::tuple<int, int, int>> GetNeighboursIn(std::tuple<int, int, int> hexCoordinates, int distance =1,int minDistance = 1);

	void drawBoard(sf::RenderTarget& target);
	void deleteHexagons();
	void setStart();
	void setRocks();
	void setBases();
	void boardPreperation();
private:
	int numRows;
	int numCols;
	float hexSize;
	int numHexagons;
	float vertSpacing;
	float horizSpacing;
	float xOffset;
	float yOffset;
};
