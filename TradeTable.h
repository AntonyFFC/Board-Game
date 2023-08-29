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
class TradeTable
{
public:
	TradeTable(Pawn* inPawn, Pawn* inBody, sf::RenderWindow* window);
	void draw();
	bool tableClicked(sf::Vector2i mousePosition);
	bool doneClicked(sf::Vector2i mousePosition);
	void trade(sf::Vector2i mousePosition);

private:
	void createTexture(std::vector<Equipment*> equipment, bool bodys);
	void drawDoneButton();
	bool isOnBody(sf::Vector2i mousePosition);
	Equipment* clickOnEquipment(sf::Vector2i mousePosition, std::vector<Equipment*> equipment);
	void tradeItem(Equipment* item, bool isBodys);
	sf::Text initializeTradeText();
	void initializeTexture();
	void setUpDimensions();

	Pawn* pawn;
	Pawn* body;
	sf::RenderWindow* target;
	std::vector<std::function<std::string(const Equipment&)>> functions;
	sf::RectangleShape cell;
	sf::RectangleShape doneCell;
	sf::Text tradeText;
	std::map<std::string, sf::Sprite> iconSprites;
	std::vector<sf::Texture*> iconTextures;
	std::string headers[8];
	int minX;
	int minY;
	int maxX;
	int maxY;
	int cellWidths[8];
	int sumOfCellWidths;
	int cellHeight;
	float gap;
	sf::RenderTexture tableRenderTexture;
	sf::Sprite tableSprite;
};

