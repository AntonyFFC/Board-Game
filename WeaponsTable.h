#pragma once
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include "Board.h"
#include "Pawn.h"
#include <tuple>
#include <functional>
#include <iostream>
class WeaponsTable
{
public:
	WeaponsTable(Pawn* inPawn, sf::RenderWindow* window);
	void draw();
	bool tableClicked(sf::Vector2i mousePosition);

private:
	void drawTypeIcon(std::string type);
	void drawSpaceIcon(Equipment::SpaceOccupied space);
	Equipment* clickOnEquipment(sf::Vector2i mousePosition);

	std::map<std::string, sf::Sprite> initializeSpriteMap();
	std::vector<std::function<std::string(const Equipment&)>> initializeFunctions();
	sf::RectangleShape initializeCells();
	sf::Text initializeText();
	void setUpDimensions();

	std::vector<Equipment*> weapons;
	sf::RenderWindow* target;
	std::vector<std::function<std::string(const Equipment&)>> functions;
	sf::RectangleShape cell;
	sf::RectangleShape doneCell;
	sf::Text text;
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

