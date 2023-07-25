#pragma once
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include "Board.h"
#include "Pawn.h"
#include <tuple>
#include <functional>
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
	bool isOnBody(sf::Vector2i mousePosition);
	void clickOnBodysEquipment(sf::Vector2i mousePosition);
	void clickOnPawnsEquipment(sf::Vector2i mousePosition);

	std::map<std::string, sf::Sprite> initializeSpriteMap();
	std::vector<std::function<std::string(const Equipment&)>> initializeFunctions();
	sf::RectangleShape initializeCells();
	sf::Text initializeTradeText();

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
};

