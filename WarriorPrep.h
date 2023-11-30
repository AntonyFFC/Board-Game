#pragma once

#include "Pawn.h"
#include <SFML/Graphics.hpp>
#include "Globals.h"
#include "TableUtils.h"
class WarriorPrep
{
public:
	WarriorPrep();
	~WarriorPrep();

	void start();
	void addPawns(std::vector<Pawn*> pawns);
private:
	std::vector<Pawn*> pawnDict;
	sf::Text text;
	int fontSize;
};

