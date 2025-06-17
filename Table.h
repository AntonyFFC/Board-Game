#pragma once
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include <tuple>
#include <functional>
#include "TableUtils.h"
#include "SpriteUtils.h"
class Table
{
public:
	Table(sf::RenderWindow* inWindow);
	~Table();

	void draw();
	bool tableClicked(sf::Vector2i mousePosition);
	void setPosition(sf::Vector2f inPosition);
	void setEquipment(std::vector<Equipment*> inEquipment);
	sf::Vector2f getPosition() const;
	sf::Vector2f getSize() const;
	Equipment* getClickedItem(sf::Vector2i mousePosition);
	void createTexture();
	sf::Sprite getTableSprite();
	void toggleHighlightClickeditem(sf::Vector2i mousePosition);
	std::vector<Equipment*> getHighlightedItems() const;
	void unhighlightAllItems();
	int cellHeight;

private:
	void drawHeaders();
	void drawEquipment();
	void setUpDimensions();
	Equipment* clickOnEquipment(sf::Vector2i mousePosition);

	std::vector<Equipment*> equipment;
	sf::RenderWindow* target;
	std::vector<std::function<std::string(const Equipment&)>> functions;
	sf::RectangleShape cell;
	sf::Text text;
	std::map<std::string, sf::Sprite> iconSprites;
	std::vector<sf::Texture*> iconTextures;
	sf::Vector2f position;
	std::string headers[8];
	int minX;
	int minY;
	int maxX;
	int maxY;
	int cellWidths[8];
	int sumOfCellWidths;
	int numberOfItems;
	sf::RenderTexture tableRenderTexture;
	sf::Sprite tableSprite;
	std::vector<int> highlightedEquipmentIndices;
};