#pragma once
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include "Board.h"
#include "Pawn.h"
#include <tuple>
#include <functional>
#include <iostream>
class TradeTable
{
public:
	TradeTable(Pawn* inPawn, Pawn* inBody, sf::RenderWindow* window);
	void draw();
	bool tableClicked(sf::Vector2i mousePosition);
	void trade(sf::Vector2i mousePosition);


private:
	void drawTable(std::vector<Equipment*> equipment, bool bodys);
	void drawTypeIcon(std::string type);
	void drawSpaceIcon(Equipment::SpaceOccupied space);
	void drawDoneButton();
	bool isOnBody(sf::Vector2i mousePosition);
	Equipment* clickOnEquipment(sf::Vector2i mousePosition, std::vector<Equipment*> equipment);
	void tradeItem(Equipment* item, bool isBodys);

	std::map<std::string, sf::Sprite> initializeSpriteMap();
	std::vector<std::function<std::string(const Equipment&)>> initializeFunctions();
	sf::RectangleShape initializeCells();
	sf::Text initializeTradeText();
	void setUpDimensions();

	Pawn* pawn;
	Pawn* body;
	sf::RenderWindow* target;
	std::vector<std::function<std::string(const Equipment&)>> functions;
	sf::RectangleShape cell;
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
};

