#include "Card.h"
#include <algorithm>

namespace {
    const sf::Color kParchmentBg{210, 188, 148};
    const sf::Color kHeaderFill{88, 42, 36};
    const sf::Color kBrassBorder{196, 154, 72};
    const sf::Color kCellBorder{150, 112, 52};
    const sf::Color kDescriptionBg{58, 38, 26};
    const sf::Color kSelectionTint{255, 210, 90, 140};
    const sf::Color kHoverTint{255, 248, 220};
}

template <typename Container>
int getSumOfVector(const Container& vec) {
	int sum = 0;
	for (const auto& value : vec) {
		sum += value;
	}
	return sum;
}

const int Card::fontSize = 23;
const int Card::cellHeight = 35;
const float Card::scaleFactor = 0.15f;
std::vector<sf::Texture*> Card::iconTextures;
std::map<std::string, sf::Sprite> Card::iconSprites = initializeSpriteMap(iconTextures);


Card::Card(const std::vector<int>& widths, const std::vector<std::string>& headers, sf::Vector2f pos)
	: cellWidths(widths), headers(headers)
{
	initializeFont();
	cell = initializeCells();
	position = pos;
	text = initializeText("card", &globalFont2, fontSize, sf::Color::White);
	cell = initializeCells();
	sumOfCellWidths = getSumOfVector(cellWidths);
	highlightColor = kSelectionTint;
	isBeingClicked = false;
	hovered = false;
}

Card::~Card()
{
	delete combinedTexture;
}

template <typename RenderType>
void Card::draw(RenderType* window)
{
	window->draw(cardSprite);
}

sf::Sprite Card::getFullSprite()
{
	if (isBeingClicked)
	{
		return combinedSprite;
	}
	return cardSprite;
}

sf::Sprite Card::getSprite()
{
	return cardSprite;
}

sf::Sprite Card::getHighSprite()
{
	return highlightSprite;
}

void Card::setPosition(sf::Vector2f pos)
{
	position = pos;
	cardSprite.setPosition(position);
	combinedSprite.setPosition(position);
	highlightSprite.setPosition(position);
}

void Card::setHovered(bool hovered)
{
	this->hovered = hovered;
}

bool Card::isHovered() const
{
	return hovered;
}

void Card::drawToTarget(sf::RenderTarget& target) const
{
	if (isBeingClicked) {
		target.draw(combinedSprite);
		return;
	}

	sf::Sprite display = cardSprite;
	if (hovered) {
		display.setColor(kHoverTint);
	}
	target.draw(display);

	if (hovered) {
		drawHoverOutline(target);
	}
}

void Card::drawHoverOutline(sf::RenderTarget& target) const
{
	const sf::FloatRect bounds = cardSprite.getGlobalBounds();
	sf::RectangleShape outline;
	outline.setPosition(bounds.left - 3.0f, bounds.top - 3.0f);
	outline.setSize(sf::Vector2f(bounds.width + 6.0f, bounds.height + 6.0f));
	outline.setFillColor(sf::Color::Transparent);
	outline.setOutlineColor(sf::Color(255, 220, 120));
	outline.setOutlineThickness(3.0f);
	target.draw(outline);
}

void Card::drawHeldPreview(sf::RenderTarget& target, sf::Vector2f anchor) const
{
	sf::Sprite preview = cardSprite;
	const sf::Vector2f originalScale = preview.getScale();
	const float heldScaleFactor = 0.6f;
	preview.setScale(originalScale.x * heldScaleFactor, originalScale.y * heldScaleFactor);
	preview.setColor(sf::Color(255, 255, 255, 190));

	const sf::FloatRect localBounds = preview.getLocalBounds();
	preview.setOrigin(localBounds.width / 2.0f, localBounds.height / 2.0f);
	preview.setPosition(anchor.x + 28.0f, anchor.y + 28.0f);
	target.draw(preview);

	const sf::FloatRect bounds = preview.getGlobalBounds();
	sf::RectangleShape outline;
	outline.setPosition(bounds.left - 2.0f, bounds.top - 2.0f);
	outline.setSize(sf::Vector2f(bounds.width + 4.0f, bounds.height + 4.0f));
	outline.setFillColor(sf::Color::Transparent);
	outline.setOutlineColor(sf::Color(255, 220, 100, 210));
	outline.setOutlineThickness(2.0f);
	target.draw(outline);
}

void Card::click(bool boolean)
{
	isBeingClicked = boolean;
}

void Card::movePosition(sf::Vector2f pos)
{
	position += pos;
	cardSprite.setPosition(position);
}

void Card::setScale(float scl)
{
	cardSprite.setScale(scl, scl);
	combinedSprite.setScale(scl, scl);
	highlightSprite.setScale(scl, scl);
}

bool Card::isClicked(sf::Vector2i mousePosition) const
{
	return cardSprite.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosition));
}

bool Card::isClicked(sf::Vector2f mousePosition) const
{
	return cardSprite.getGlobalBounds().contains(mousePosition);
}

bool Card::isBeingClkd() const
{
	return isBeingClicked;
}

void Card::createSprite()
{
	renderTexture.clear(kParchmentBg);
	createTexture();
	drawCardFrame();
	renderTexture.display();
	cardSprite = sf::Sprite(renderTexture.getTexture());
	cardSprite.setPosition(position);
	highlightSprite = sf::Sprite(cardSprite);
	highlightSprite.setColor(highlightColor);
}

void Card::createTexture()
{
	drawHeaders();
	drawValues();
	drawPicture();
}

void Card::drawDescriptionBackground(float yOffset)
{
	const float height = static_cast<float>(renderTexture.getSize().y) - yOffset;
	sf::RectangleShape bg;
	bg.setPosition(0.0f, yOffset);
	bg.setSize(sf::Vector2f(static_cast<float>(sumOfCellWidths), height));
	bg.setFillColor(kDescriptionBg);
	bg.setOutlineColor(kCellBorder);
	bg.setOutlineThickness(1.0f);
	renderTexture.draw(bg);
}

void Card::drawCardFrame()
{
	const sf::Vector2u size = renderTexture.getSize();
	sf::RectangleShape frame;
	frame.setSize(sf::Vector2f(static_cast<float>(size.x), static_cast<float>(size.y)));
	frame.setFillColor(sf::Color::Transparent);
	frame.setOutlineColor(kBrassBorder);
	frame.setOutlineThickness(4.0f);
	renderTexture.draw(frame);
}

void Card::createCombinedSprite()
{
	sf::Vector2u size = renderTexture.getSize();
	sf::RenderTexture renderTexture;
	renderTexture.create(size.x, size.y);
	renderTexture.clear();

	renderTexture.draw(cardSprite);
	renderTexture.draw(highlightSprite);
	renderTexture.display();

	combinedTexture = new sf::Texture(renderTexture.getTexture());
	combinedSprite = sf::Sprite(*combinedTexture);
}

void Card::drawHeaders()
{
	sf::Vector2f initialPos(0, 0);
	cell.setPosition(initialPos);
	setPosSpriteMap(initialPos.x + 3, initialPos.y + 3, iconSprites);
	setScalSpriteMap(0.06, iconSprites);
	text.setPosition(initialPos.x + 5, initialPos.y + 3);
	cell.setFillColor(kHeaderFill);
	cell.setOutlineColor(kBrassBorder);
	cell.setOutlineThickness(1.0f);
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

void Card::drawPicture(sf::Vector2f initialPos)
{
	pictureSprite.setScale(scaleFactor, scaleFactor);
	//this is so that I set the x coordinate of the center but the y coordinate of the top edge
	sf::Vector2f finalPos(initialPos.x + (sumOfCellWidths)/2 - pictureSprite.getGlobalBounds().width / 2.0f,
		initialPos.y + 1.5*cellHeight);
	pictureSprite.setPosition(finalPos.x, finalPos.y);
	renderTexture.draw(pictureSprite);
}

void Card::moveBack()
{
	cell.move(-sumOfCellWidths, cellHeight);
	text.move(-sumOfCellWidths, cellHeight);
	moveSpriteMap(-sumOfCellWidths, cellHeight, iconSprites);
}

WarriorCard::WarriorCard(Pawn* warrior)
	: Card({ 220, 40, 40, 40, 40, 40 },{ "Name", "history-icon-white",
		"hand-line-icon-white", "plus-round-line-icon-white", 
		"heart-line-icon-white", "dollar-icon-white" }), warrior(warrior)
{
	renderTexture.create(sumOfCellWidths, 230);
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
	createCombinedSprite();
}

int WarriorCard::getPrice() const
{
	return warrior->getPrice();
}

Pawn* WarriorCard::getWarrior() const
{
	return warrior;
}

void WarriorCard::drawValues()
{
	cell.setFillColor(getTeamColor(warrior->getTeamNumber()));
	cell.setOutlineColor(kCellBorder);
	cell.setOutlineThickness(1.0f);
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

	drawDescriptionBackground(2.0f * cellHeight);
	text.setPosition(8.0f, 2.0f * cellHeight + 6.0f);
	text.setString(functions[6](*static_cast<const Pawn*>(warrior)));
	renderTexture.draw(text);
}

void WarriorCard::drawPicture(sf::Vector2f initialPos)
{
	Card::drawPicture(sf::Vector2f(0.f, 20.f));
}

EquipmentCard::EquipmentCard(Equipment* item)
	: Card({ 150, 60, 60, 40, 40, 40, 40 }, 
	{ "Name","left-right-arrow-icon-white","circle-line-icon-white","bomb-blast-icon-white",
	"history-icon-white","cube-icon-white","dollar-icon-white"}), item(item)
{
	renderTexture.create(sumOfCellWidths, 150);
	functions = initializeFunctions();
	pictureSprite = loadSprite(item->getName());
	createSprite();
	createCombinedSprite();
}

int EquipmentCard::getPrice() const
{
	return item->getPrice();
}

Equipment* EquipmentCard::getItem() const
{
	return item;
}

void EquipmentCard::drawValues()
{
	cell.setFillColor(getTypeColor(item));
	cell.setOutlineColor(kCellBorder);
	cell.setOutlineThickness(1.0f);
	for (int i = 0; i < 7; i++)
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
	drawDescriptionBackground(2.0f * cellHeight);
	text.setPosition(8.0f, 2.0f * cellHeight + 6.0f);
	text.setString(functions[7](*static_cast<const Equipment*>(item)));
	renderTexture.draw(text);
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
