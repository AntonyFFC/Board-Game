#include "TableUtils.h"

void drawSpaceIcon(Equipment::SpaceOccupied space, sf::RenderTexture& target, std::map<std::string, sf::Sprite>& iconSprites)
{
	int added = 0;
	for (int i = 0; i < space.numSpaces; i++)
	{
		if (space.spaceType == "hands")
		{
			target.draw(iconSprites["hand-line-icon-white"]);
			iconSprites["hand-line-icon-white"].move(15, 0);
		}
		else
		{
			target.draw(iconSprites["plus-round-line-icon-white"]);
			iconSprites["plus-round-line-icon-white"].move(15, 0);
		}
		added++;
	}
	if (space.spaceType == "hands")
	{
		iconSprites["hand-line-icon-white"].move(-15 * added, 0);
	}
	else
	{
		iconSprites["plus-round-line-icon-white"].move(-15 * added, 0);
	}

}

void drawSpaceIconBlack(Equipment::SpaceOccupied space, sf::RenderTexture& target, std::map<std::string, sf::Sprite>& iconSprites)
{
	int added = 0;
	for (int i = 0; i < space.numSpaces; i++)
	{
		if (space.spaceType == "hands")
		{
			target.draw(iconSprites["hand-line-icon"]);
			iconSprites["hand-line-icon"].move(15, 0);
		}
		else
		{
			target.draw(iconSprites["plus-round-line-icon"]);
			iconSprites["plus-round-line-icon"].move(15, 0);
		}
		added++;
	}
	if (space.spaceType == "hands")
	{
		iconSprites["hand-line-icon"].move(-15 * added, 0);
	}
	else
	{
		iconSprites["plus-round-line-icon"].move(-15 * added, 0);
	}

}

void drawTypeIcon(std::string type, sf::RenderTexture& target, std::map<std::string, sf::Sprite>& iconSprites)
{
	if (type == "Weapon")
	{
		target.draw(iconSprites["crossed-swords-icon-white"]);
	}
	else if (type == "Armour")
	{
		target.draw(iconSprites["shield-icon-white"]);
	}
	else
	{
		target.draw(iconSprites["four-squares-icon-white"]);
	}
}

void drawTypeIconBlack(std::string type, sf::RenderTexture& target, std::map<std::string, sf::Sprite>& iconSprites)
{
	if (type == "Weapon")
	{
		target.draw(iconSprites["crossed-swords-icon"]);
	}
	else if (type == "Armour")
	{
		target.draw(iconSprites["shield-icon"]);
	}
	else
	{
		target.draw(iconSprites["four-squares-icon"]);
	}
}

std::map<std::string, sf::Sprite> initializeSpriteMap(std::vector<sf::Texture*>& iconTextures)
{
	std::string folderPath = "assets/icons/";
	std::string searchPattern = folderPath + "*.png";
	WIN32_FIND_DATAA findData;
	HANDLE findHandle = FindFirstFileA(searchPattern.c_str(), &findData);

	std::map<std::string, sf::Sprite> sprites;

	if (findHandle != INVALID_HANDLE_VALUE)
	{
		do
		{
			std::string fileName = findData.cFileName;
			fileName = fileName.substr(0, fileName.length() - 4);
			std::string filePath = folderPath + fileName + ".png";

			// Load the texture
			sf::Texture* texture;
			texture = new sf::Texture;
			if (!texture->loadFromFile(filePath))
			{
				delete texture;
				continue;
			}
			// This holds the textures so that the sprites work
			iconTextures.push_back(texture);
			sf::Sprite sprite(*texture);
			sprites[fileName] = sprite;
		} while (FindNextFileA(findHandle, &findData));
		FindClose(findHandle);
	}
	else
	{
		throw std::runtime_error("Invalid handle value");
	}
	return sprites;
}

std::vector<std::function<std::string(const Equipment&)>> initializeFunctions()
{
	std::vector<std::function<std::string(const Equipment&)>> newFunctions;
	newFunctions.push_back([](const Equipment& item) { return item.getName(); });
	newFunctions.push_back([](const Equipment& item) { return item.rangeToString(); });
	newFunctions.push_back([](const Equipment& item) { return item.spaceToString(); }); //here draw the sprite with space
	newFunctions.push_back([](const Equipment& item) { return std::to_string(item.getAttackValue()); });
	newFunctions.push_back([](const Equipment& item) { return std::to_string(item.getAttackActions()); });
	newFunctions.push_back([](const Equipment& item) { return item.getType(); }); //here draw the sprite with type
	newFunctions.push_back([](const Equipment& item) { return std::to_string(item.getPrice()); });
	newFunctions.push_back([](const Equipment& item) { return item.getAdditionalCapabilities(); });
	return newFunctions;
}

std::vector<std::function<std::string(const Pawn&)>> initializePawnFunctions()
{
	std::vector<std::function<std::string(const Pawn&)>> newFunctions;
	newFunctions.push_back([](const Pawn& pawn) { return pawn.getName(); });
	newFunctions.push_back([](const Pawn& pawn) { return std::to_string(pawn.getMaxActions()); });
	newFunctions.push_back([](const Pawn& pawn) { return std::to_string(pawn.getSpace().hands); });
	newFunctions.push_back([](const Pawn& pawn) { return std::to_string(pawn.getSpace().extras); });
	newFunctions.push_back([](const Pawn& pawn) { return std::to_string(pawn.getHP()); });
	newFunctions.push_back([](const Pawn& pawn) { return std::to_string(pawn.getPrice()); });
	return newFunctions;
}

sf::RectangleShape initializeCells(float width, float height, float x, float y)
{
	sf::RectangleShape newCell;
	newCell.setFillColor(sf::Color::Transparent);
	newCell.setOutlineColor(sf::Color::White);
	newCell.setOutlineThickness(1.f);
	newCell.setSize(sf::Vector2f(width, height));
	newCell.setPosition(x, y);
	return newCell;
}

void setScalCells(float scl, std::vector <sf::RectangleShape*>& cells)
{
	for (auto& cell : cells)
	{
		sf::RectangleShape* currentCell = cell;
		currentCell->setScale(scl, scl);
	}
}

void setScalTexts(float scl, std::vector <sf::Text*>& texts)
{
	for (auto& text : texts)
	{
		sf::Text* currentText = text;
		currentText->setScale(scl, scl);
	}
}

void setPosCells(int x, int y, std::vector <sf::RectangleShape*>& cells)
{
	for (auto& cell : cells)
	{
		sf::RectangleShape* currentCell = cell;
		currentCell->setPosition(x, y);
	}
}

void setPosTexts(int x, int y, std::vector <sf::Text*>& texts)
{
	for (auto& text : texts)
	{
		sf::Text* currentText = text;
		currentText->setPosition(x, y);
	}
}

void moveCells(int addx, int addy, std::vector <sf::RectangleShape*>& cells)
{
	for (auto& cell : cells)
	{
		sf::RectangleShape* currentCell = cell;
		currentCell->move(addx, addy);
	}
}

void moveTexts(int addx, int addy, std::vector <sf::Text*>& texts)
{
	for (auto& text : texts)
	{
		sf::Text* currentText = text;
		currentText->move(addx, addy);
	}
}

void setSizeCells(float xSiz, float ySiz, std::vector <sf::RectangleShape*>& cells)
{
	for (auto& cell : cells)
	{
		sf::RectangleShape* currentCell = cell;
		currentCell->setSize(sf::Vector2f(xSiz, ySiz));
	}
}

void setFillColorCells(sf::Color color, std::vector <sf::RectangleShape*>& cells)
{
	for (auto& cell : cells)
	{
		sf::RectangleShape* currentCell = cell;
		currentCell->setFillColor(color);
	}
}

sf::Color getTypeColor(Equipment* item)
{
	std::string type = item->getType();

	if (type == "Weapon") {
		return sf::Color(46, 46, 46);
	}
	else if (type == "Armour") {
		return sf::Color(122, 0, 6);
	}
	else {
		return sf::Color(99, 94, 0);
	}
}

sf::Color getTeamColor(int team)
{
	switch (team)
	{
	case 0:
		return sf::Color(145, 4, 4);
	case 1:
		return sf::Color(28, 36, 82);
	case 2:
		return sf::Color(28, 82, 42);
	case 3:
		return sf::Color(145, 145, 4);
	case 4:
		return sf::Color(84, 6, 80);
	case 5:
		return sf::Color(5, 121, 171);
	default:
		return sf::Color::White;
	}
}

sf::Text initializeText(std::string content, sf::Font* font, float fontSize, sf::Color color)
{
	sf::Text newText(content, *font, fontSize);
	newText.setFillColor(color);
	return newText;
}
