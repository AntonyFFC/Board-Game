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
class WeaponsTable
{
public:
	WeaponsTable(Pawn* inPawn, Pawn* inAttacked, std::vector<Equipment*> weapons, sf::RenderWindow* inWindow);
	void draw();
	bool tableClicked(sf::Vector2i mousePosition);
	Pawn* getAttacker() const;
	Pawn* getAttacked() const;
	Equipment* getWeapon(sf::Vector2i mousePosition);

private:
	void createTexture();
	void drawHeaders();
	void drawEquipment();
	Equipment* clickOnEquipment(sf::Vector2i mousePosition);
	void initializeTexture();
	void setUpDimensions();

	Pawn* attacker;
	Pawn* attacked;
	std::vector<Equipment*> weapons;
	sf::RenderWindow* target;
	std::vector<std::function<std::string(const Equipment&)>> functions;
	sf::RectangleShape cell;
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
	sf::RenderTexture tableRenderTexture;
	sf::Sprite tableSprite;
};

