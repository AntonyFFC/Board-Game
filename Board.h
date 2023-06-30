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

	std::map<std::tuple<int, int, int>, Hex*> hexDict; // stores al of the hexes in the dictionary with the coordinates as the index
	std::map<std::tuple<int, int, int>, std::vector<std::tuple<int, int, int>>> hexNeighboursDict; // stores all neighbours of each hex

	void drawBoard(sf::RenderTarget& target, bool isShift);
private:
	std::vector < std::tuple<int, int, int>> getNeighbours(std::tuple<int, int, int> hexCoordinates);
	std::vector < std::tuple<int, int, int>> getInRange(std::tuple<int, int, int> hexCoordinates, int dist = 1, int minDistance = 1);
	std::vector < std::tuple<int, int, int>> getReachable(std::tuple<int, int, int> start, int movement = 5);
	std::vector < std::tuple<int, int, int>> linedraw(std::tuple<int, int, int> a, std::tuple<int, int, int> b);
	std::vector < std::tuple<int, int, int>> getInView(std::tuple<int, int, int> start, int dist = 1, int minDist = 1);

	void deleteHexagons();
	void setStart();
	void setRocks();
	void setBases();
	void boardPreperation();
	void clearHighlight();
	void setWall(std::tuple<int, int, int> coords);
	void setGrass(std::tuple<int, int, int> coords);

	int numRows;
	int numCols;
	float hexSize;
	int numHexagons;
	float vertSpacing;
	float horizSpacing;
	float xOffset;
	float yOffset;
	std::vector<std::vector< std::tuple<int, int, int>>> highlighted;
	friend class Pawns;
};
