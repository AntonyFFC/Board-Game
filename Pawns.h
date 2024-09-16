#pragma once
#include <SFML/Graphics.hpp>
#include "Hex.h"
#include <map>
#include <tuple>
#include "Pawn.h"
#include "Board.h"
#include "Equipment.h"
#include <functional>
#include <iostream>
#include "TradeTable.h"
#include "Table.h"
#include "Walls.h"

class Pawns
{
public:
	Pawns(Board* board, sf::RenderWindow* window);
	~Pawns();

	void handleClick(sf::Vector2i mousePosition);
	void handleClickRight(sf::Vector2i mousePosition);
	void addPawn(Pawn* inPawn);
	void addWalls(int numWalls, int teamNum);
	void handleShiftOn();
	void handleShiftOff();
	bool isTrading() const;
	bool isChoosing() const;
	bool addItemToPawn(int number, Equipment* item);
	template<typename... Args>
	bool addItemToPawn(int number, Equipment* item, Args&&... rest)
	{
		bool done;
		if (!pawnDict[number]->addEquipment(item))
		{
			done = false;
		}
		else {
			done = true;
		}
		addItemToPawn(number, std::forward<Args>(rest)...); //reccursion
		return done;
	}
	void setTrading(bool boolean);
	void setChoosing(bool boolean);
	void draw(bool isShift);

private:
	void pawnClicked(int pawnNum);
	void trading(sf::Vector2i mousePosition);
	void choosing(sf::Vector2i mousePosition);
	void pawnMoved(int pawnNum);
	void attack(int pawnNum, int attackedNum, Equipment* weapon);
	void death(Pawn* attacked);
	std::vector<Equipment*> getWeaponsInUse(int pawnNum, int attackedNum);
	bool pawnFirst();
	void pawnSecond(int pawnNum); //second click, number of previous pawn
	std::vector<std::tuple<int, int, int>> getViewOfWeapon(int pawnNum, Equipment* weapon);
	std::vector<std::tuple<int, int, int>> getViewOfPawn(int pawnNum);
	std::vector<std::tuple<int, int, int>> getRangeOfPawn(int pawnNum);
	void setupText();
	void flipTurn();
	int numberOfPawn(std::tuple<int, int, int> coords, bool body = false);
	bool placeWall(int pawnNumber, std::tuple<int, int, int> coords);
	bool destroyWall(int pawnNumber, std::tuple<int, int, int> coords);
	void drawTurn();
	void drawPawns(bool isShift);
	void resetTurn(int pawnNum);

	Board* board;
	TradeTable* tradeTable;
	Table* currentTable;
	Walls* walls;
	sf::RenderWindow* target;
	std::tuple<int, int, int> previousHex;
	bool wasShift;
	std::tuple<int, int, int> empty = { -9, -9, -9 };
	std::vector<Pawn*> pawnDict;
	int whosTurn;
	int whichPawn; //current pawn number in pawnDict
	sf::Text turnText;
	bool isTrading_;
	bool isChoosing_;
	std::tuple<int, int, int> current; //current clicked hex
	int previousWarrior[2];
	int numberOfPawns[2];
};

