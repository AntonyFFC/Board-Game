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

void WarriorPrep::start()
{
}

void WarriorPrep::addPawns(std::vector<Pawn*> pawns)
{
	pawnDict = pawns;
}
