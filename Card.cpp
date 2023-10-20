#include "Card.h"

template <typename Container>
int getSumOfVector(const Container& vec) {
	int sum = 0;
	for (const auto& value : vec) {
		sum += value;
	}
	return sum;
}

Card::Card(const std::vector<int>& widths, const std::vector<std::string>& headers) 
	: cellWidths(widths), headers(headers)
{
	initializeFont();
	cell = initializeCells();
	fontSize = 20;
	cellHeight = 30;
	position = sf::Vector2f(0, 0);
	iconSprites = initializeSpriteMap(iconTextures);
	cell = initializeCells();
	sumOfCellWidths = getSumOfVector(cellWidths);
}

template <typename RenderType>
void Card::draw(RenderType* window)
{
	window->draw(cardSprite);
}

sf::Sprite Card::getSprite()
{
	return cardSprite;
}

bool Card::isClicked(sf::Vector2i mousePosition)
{
	return false; // do this
}

void Card::createSprite()
{
	renderTexture.clear(sf::Color::Transparent);
	createTexture();
	renderTexture.display();
	cardSprite = sf::Sprite(renderTexture.getTexture());
}

void Card::createTexture()
{
	drawHeaders();
	drawValues();
	drawPicture();
}

void Card::drawHeaders()
{
	cell.setPosition(position);
	setPosSpriteMap(position.x + 3, position.y + 3, iconSprites);
	setScalSpriteMap(0.04, iconSprites);
	text.setPosition(position.x + 5, position.y + 3);
	cell.setFillColor(sf::Color(156, 84, 84));
	for (int i = 0; i < headers.size(); i++)
	{
		cell.setSize(sf::Vector2f(cellWidths[i], cellHeight));
		renderTexture.draw(cell);
		if (headers[i].length() < 9)
		{
			text.setString(headers[i]);
			renderTexture.draw(text);
		}
		else
		{
			renderTexture.draw(iconSprites[headers[i]]);
		}
		cell.move(cellWidths[i], 0);
		text.move(cellWidths[i], 0);
		moveSpriteMap(cellWidths[i], 0, iconSprites);
	}
	moveBack();
}

void Card::drawPicture()
{
	pictureSprite.setPosition(position.x + 3, position.y + 50);
	renderTexture.draw(pictureSprite);
}

void Card::moveBack()
{
	cell.move(-sumOfCellWidths, cellHeight);
	text.move(-sumOfCellWidths, cellHeight);
	moveSpriteMap(-sumOfCellWidths, cellHeight, iconSprites);
}

WarriorCard::WarriorCard(Pawn* warrior)
	: Card({ 250, 50, 50, 50, 50, 50 },{ "Name", "history-icon-white",
		"hand-line-icon-white", "plus-round-line-icon-white", 
		"heart-line-icon-white", "dollar-icon-white" }), warrior(warrior)
{
	renderTexture.create(500, 300);
	functions = initializePawnFunctions();
	if (warrior->getSide() == 0)
	{
		pictureSprite = loadSprite("red");
	}
	else
	{
		pictureSprite = loadSprite("blue");
	}
	createSprite();
}

void WarriorCard::drawValues()
{
	cell.setFillColor(getTeamColor(warrior->getTeamNumber()));
	for (int i = 0; i < 6; i++)
	{
		cell.setSize(sf::Vector2f(cellWidths[i], cellHeight));
		renderTexture.draw(cell);

		text.setString(functions[i](*static_cast<const Pawn*>(warrior)));
		renderTexture.draw(text);

		cell.move(cellWidths[i], 0);
		text.move(cellWidths[i], 0);
		moveSpriteMap(cellWidths[i], 0, iconSprites);
	}
	moveBack();
}

EquipmentCard::EquipmentCard(Equipment* item)
	: Card({ 150, 60, 60, 50, 50, 50, 50, 500 }, 
	{ "Name","left-right-arrow-icon-white","circle-line-icon-white","bomb-blast-icon-white",
	"history-icon-white","cube-icon-white","dollar-icon-white","Other" }), item(item)
{
	renderTexture.create(500, 300);
	functions = initializeFunctions();
	pictureSprite = loadSprite(item->getName());
	createSprite();
}

void EquipmentCard::drawValues()
{
	cell.setFillColor(getTypeColor(item));
	for (int i = 0; i < 8; i++)
	{
		cell.setSize(sf::Vector2f(cellWidths[i], cellHeight));
		renderTexture.draw(cell);
		if (i == 2)
		{
			drawSpaceIcon(item->getSpaceOccupied(), renderTexture, iconSprites);
		}
		else if (i == 5)
		{
			drawTypeIcon(item->getType(), renderTexture, iconSprites);
		}
		else
		{
			text.setString(functions[i](*static_cast<const Equipment*>(item)));
			renderTexture.draw(text);
		}
		cell.move(cellWidths[i], 0);
		text.move(cellWidths[i], 0);
		moveSpriteMap(cellWidths[i], 0, iconSprites);
	}
	moveBack();
}

sf::Sprite Card::loadSprite(const std::string& textureName)
{
	std::string folderPath = "assets/equipment/";
	std::string filePath = folderPath + textureName + ".png";

	if (!pictureTexture.loadFromFile(filePath))
	{
		throw std::runtime_error("Failed to load texture: " + textureName);
	}

	sf::Sprite sprite(pictureTexture);
	return sprite;
}
