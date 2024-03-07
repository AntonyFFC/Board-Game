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

private:
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