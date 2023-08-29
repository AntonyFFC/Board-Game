#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "Equipment.h"
#include "EquipmentManager.h"
#include "Globals.h"
#include "SpriteUtils.h"
#include "TableUtils.h"
class Armory
{
public:
	Armory(sf::RenderWindow* window);
	~Armory();
	void start();
	void exit();
private:
	void createTexture();
	void drawHeaders();
	void drawEquipment();
	sf::Color getTypeColor(Equipment* item);
	void drawBackButton();
	void keyPressed(const sf::Event& event);
	bool backClicked(sf::Vector2i mousePosition);

	void initializeText();
	std::string filename;
	sf::RenderWindow* window;
	std::vector<Equipment*> equipmentList;
	sf::RectangleShape cell;
	sf::RectangleShape backCell;
	sf::Text text;
	sf::Text titleText;
	std::map<std::string, sf::Sprite> iconSprites;
	std::vector<sf::Texture*> iconTextures;
	std::string headers[8];
	std::vector<std::function<std::string(const Equipment&)>> functions;
	int cellWidths[8];
	int sumOfCellWidths;
	int cellHeight;
	int fontSize;
	sf::Vector2f position;
	sf::RenderTexture tableRenderTexture;
	bool closed;
};
