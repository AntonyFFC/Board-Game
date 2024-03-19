#pragma once
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include <tuple>
#include <functional>
#include "TableUtils.h"
class Table
{
public:
	Table();
	~Table();

	void draw();
	bool tableClicked(sf::Vector2i mousePosition);

private:
	void createTexture();
	void drawHeaders();
	void setUpDimensions();

	sf::RenderWindow* target;
	sf::RectangleShape cell;
	sf::Text text;
	std::map<std::string, sf::Sprite> iconSprites;
	std::vector<sf::Texture*> iconTextures;
	std::string headers[8];
	sf::Vector2f position;
	int minX;
	int minY;
	int maxX;
	int maxY;
	int cellWidths[8];
	int sumOfCellWidths;
	int cellHeight;
	int numberOfItems;
	sf::RenderTexture tableRenderTexture;
	sf::Sprite tableSprite;
};