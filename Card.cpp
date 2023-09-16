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

Card::~Card()
{
}

template <typename RenderType>
void Card::draw(RenderType* window)
{
	window->draw(sprite);
}

sf::Sprite Card::getSprite()
{
	return sprite;
}

void Card::createSprite()
{
	renderTexture.clear(sf::Color::Transparent);
	createTexture();
	renderTexture.display();
	sprite = sf::Sprite(renderTexture.getTexture());

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
		sprite = loadSprite("red");
	}
	else
	{
		sprite = loadSprite("blue");
	}
}

EquipmentCard::EquipmentCard(Equipment* item)
	: Card({ 150, 60, 60, 50, 50, 50, 50, 500 }, 
	{ "Name","left-right-arrow-icon-white","circle-line-icon-white","bomb-blast-icon-white",
	"history-icon-white","cube-icon-white","dollar-icon-white","Other" }), item(item)
{
	renderTexture.create(500, 300);
	functions = initializeFunctions();
	sprite = loadSprite(item->getName());
}

sf::Sprite Card::loadSprite(const std::string& textureName)
{
	std::string folderPath = "assets/equipment/";
	std::string filePath = folderPath + textureName + ".png";

	if (!texture.loadFromFile(filePath))
	{
		throw std::runtime_error("Failed to load texture: " + textureName);
	}

	sf::Sprite sprite(texture);
	return sprite;
}
