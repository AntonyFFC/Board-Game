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
	WarriorPrep(Board* grid, Pawns* pawns);
	~WarriorPrep() = default;

	void rebuildLayout(const sf::FloatRect& root, const sf::FloatRect& playArea);
	void handleEvent(const sf::Event& event, sf::Vector2f logicalMouse);
	void draw(sf::RenderTarget& target);

	void addPawns(std::vector<Pawn*> pawns, int playerIndx);
	bool isFinished() const { return preperationDone; }

private:
	void drawText(sf::RenderTarget& target);
	void handleClick(sf::Vector2f logicalMouse);
	void putPawn(std::tuple<int, int, int> coordinates);

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
	sf::FloatRect promptArea_;
};
