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
class Pawns
{
public:
	Pawns(Board* board);

	void handleClick(sf::Vector2i mousePosition, sf::RenderTarget& target);
	void handleClickRight(sf::Vector2i mousePosition);
	void addPawn(Pawn* inPawn, std::tuple<int, int, int> coords);
	void handleShiftOn();
	void handleShiftOff();
	bool isTrading() const;
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
	void drawTexts(sf::RenderTarget& target);

private:
	void pawnClicked(int pawnNum);
	void pawnMoved(int pawnNum, std::tuple<int, int, int> where);
	void attack(int pawnNum, int attackedNum);
	void highlightedNoPawn(int pawnNum, std::tuple<int, int, int> current);
	bool pawnFirst(int pawnNum, std::tuple<int, int, int> current);
	void pawnSecond(int pawnNum, std::tuple<int, int, int> current);
	std::vector<std::tuple<int, int, int>> getViewOfWeapon(int pawnNum, Equipment* weapon);
	std::vector<std::tuple<int, int, int>> getViewOfPawn(int pawnNum);
	std::vector<std::tuple<int, int, int>> getRangeOfPawn(int pawnNum);
	void setupText();
	void flipTurn();
	int numberOfPawn(std::tuple<int, int, int> coords, bool body = false);
	void placeWall(int pawnNumber, std::tuple<int, int, int> coords);
	bool destroyWall(int pawnNumber, std::tuple<int, int, int> coords);
	void drawTurn(sf::RenderTarget& target);
	//trade methods
	void drawTrade(sf::RenderTarget& target);
	void drawTable(sf::RenderTarget& target, std::vector<Equipment*> equipment, int x);
	std::map<std::string, sf::Sprite> initializeSpriteMap();
	void drawTypeIcon(sf::RenderTarget& target, std::string type);
	void drawSpaceIcon(sf::RenderTarget& target, Equipment::SpaceOccupied space);
	bool tableClicked(sf::Vector2i mousePosition, sf::RenderTarget& target);

	Board* board;
	std::tuple<int, int, int> previous;
	bool wasShift;
	std::tuple<int, int, int> empty = { -9, -9, -9 };
	std::vector<Pawn*> pawnDict;
	int whosTurn;
	int whichPawn; //current pawn number in pawnDict
	sf::Text turnText;
	bool isTrading_;
	std::map<std::string, sf::Sprite> iconSprites;
	std::vector<sf::Texture*> iconTextures;
	int cellSizes[8];
	int tableY;
	int cellHeight;
};

