#include "WarriorPrep.h"

WarriorPrep::WarriorPrep()
{
	fontSize = 20;
	text = initializeText("Blue choose your staring positions", &globalFont2, 
		fontSize, sf::Color::White);
	text.setPosition(20, 10);
}

WarriorPrep::~WarriorPrep()
{
}

class StartCoordinates {
public:
	static std::vector<std::tuple<int, int, int>> blueCoordinates;
	static std::vector<std::tuple<int, int, int>> redCoordinates;

	static std::vector<std::tuple<int, int, int>> GetBlueCoordinates() {
		return blueCoordinates;
	}

	static std::vector<std::tuple<int, int, int>> GetRedCoordinates() {
		return redCoordinates;
	}
};

std::vector<std::tuple<int, int, int>> StartCoordinates::blueCoordinates = {
	{-8, 2, 6}, {-7, 2, 5}, {-7, 3, 4}, {-7, 4, 3}, {-7, 5, 2}, {-8, 6, 2},
	{-8, 5, 3}, {-8, 4, 4}, {-8, 3, 5}
};
std::vector<std::tuple<int, int, int>> StartCoordinates::redCoordinates = {
	{10, -7, -3}, {9, -6, -3}, {9, -5, -4}, {9, -4, -5}, {9, -3, -6}, {10, -3, -7},
	{10, -4, -6}, {10, -5, -5}, {10, -6, -4}
};

void WarriorPrep::start()
{
}

void WarriorPrep::addPawns(std::vector<Pawn*> pawns)
{
	pawnDict = pawns;
}