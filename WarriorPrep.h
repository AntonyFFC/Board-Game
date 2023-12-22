#pragma once

#include "Pawn.h"
#include <SFML/Graphics.hpp>
#include "Globals.h"
#include "TableUtils.h"
#include "SpriteUtils.h"
#include "Board.h"
#include "Pawns.h"
class WarriorPrep
{
public:
	WarriorPrep(sf::RenderWindow* window, Board* grid, Pawns* pawns);
	~WarriorPrep();

	void start();
	void addPawns(std::vector<Pawn*> pawns, int playerIndx);
private:
	void display();
	void drawText();
	void eventHandler(sf::Event event);
	void handleClick(sf::Vector2i mousePosition);
	void putPawn(std::tuple<int, int, int> coordinates);
	
	sf::RenderWindow* window;
	std::vector<Pawn*> pawnDict[2];
	sf::Text blueText;
	sf::Text redText;
	int fontSize;
	sf::Sprite backgroundSprite;
	sf::Texture backgroundTexture;
	Board* grid;
	Pawns* pawns;
	int currentPlayer;
	bool preperationDone;
};

