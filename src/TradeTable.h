#pragma once
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include "Board.h"
#include "Pawn.h"
#include <tuple>
#include <functional>
#include <iostream>
#include "TableUtils.h"
#include "Table.h"
#include "SpriteUtils.h"


class TradeTable
{
public:
	TradeTable(Pawn* inPawn, std::vector<Equipment*>& itemsFrom, sf::RenderWindow* window);
	~TradeTable();
	void draw();
	bool tableClicked(sf::Vector2i mousePosition);
	bool doneClicked(sf::Vector2i mousePosition);
	void trade(sf::Vector2i mousePosition);
	bool isVectorEmpty() const;

private:
	void createTexture();
	void setUpPositions();
	void drawTables();
	void drawDoneButton();
	void tradeItem(Equipment* item, bool isBodys);
	void initializeTables();
	void initializeTexture();
	void setUpDimensions();
	void setUpButton();

	Pawn* pawn;
	std::vector<Equipment*>& itemsFrom;
	sf::RenderWindow* target;
	sf::RectangleShape doneCell;
	sf::Text tradeText;
	int minX;
	int minY;
	int maxX;
	int maxY;
	float gap;
	sf::RenderTexture tableRenderTexture;
	sf::Sprite tableSprite;
	Table leftTable;     // players table
	Table rightTable;    // bodys table
};
