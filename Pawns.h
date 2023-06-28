#pragma once
#include <SFML/Graphics.hpp>
#include "Hex.h"
#include <map>
#include <tuple>
#include "Pawn.h"
#include "Board.h"
#include "Equipment.h"
class Pawns
{
public:
	Pawns(Board* board);

	void handleClick(sf::Vector2i mousePosition);
	void addPawn(Pawn* inPawn, std::tuple<int, int, int> coords);
	void handleShiftOn();
	void handleShiftOff();
	bool addItemToPawn(std::tuple<int, int, int> coords, Equipment* item);
	template<typename... Args>
	bool addItemToPawn(std::tuple<int, int, int> coords, Equipment* item, Args&&... rest)
	{
		bool done;
		if (!pawnDict[coords]->addEquipment(item))
		{
			done = false;
		}
		else {
			done = true;
		}
		addItemToPawn(coords, std::forward<Args>(rest)...); //reccursion
		return done;
	}
	void drawTurn(sf::RenderTarget& target);

private:
	void pawnClicked(std::tuple<int, int, int> current);
	void pawnMoved(std::tuple<int, int, int> previous, std::tuple<int, int, int> current);
	void attack(std::tuple<int, int, int> previous, std::tuple<int, int, int> current);
	std::vector<std::tuple<int, int, int>> getViewOfWeapon(std::tuple<int, int, int> current, Equipment* weapon);
	std::vector<std::tuple<int, int, int>> getViewOfPawn(std::tuple<int, int, int> current);
	std::vector<std::tuple<int, int, int>> getRangeOfPawn(std::tuple<int, int, int> current);
	void setupText();
	void flipTurn();

	Board* board;
	std::tuple<int, int, int> previous;
	bool wasShift;
	std::tuple<int, int, int> empty = { -9, -9, -9 };
	std::map<std::tuple<int, int, int>, Pawn*> pawnDict;
	int whosTurn;
	sf::Text turnText;
};

