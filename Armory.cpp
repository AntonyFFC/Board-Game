#include "Armory.h"

template <typename T, size_t N>
int getSumOfArray(T(&arr)[N]) {
	int sum = 0;
	for (size_t i = 0; i < N; ++i) {
		sum += arr[i];
	}
	return sum;
}

Armory::Armory(sf::RenderWindow* window)
	:window(window), equipmentHeaders{ "Name","left-right-arrow-icon-white","circle-line-icon-white","bomb-blast-icon-white",
"history-icon-white","cube-icon-white","dollar-icon-white","Other" }, pawnHeaders{ "Name","history-icon-white",
"hand-line-icon-white","plus-round-line-icon-white","heart-line-icon-white","dollar-icon-white"}, 
equipmentCellWidths{150,60,60,50,50,50,50,500}, pawnCellWidths{250, 50, 50,50, 50, 50}
{
	tableRenderTexture.create(window->getSize().x, window->getSize().y);
	tableRenderTexture.create(window->getSize().x, window->getSize().y);
	fontSize = 20;
	position = sf::Vector2f(20, 60);
	initializeFont();
	equipmentList = EquipmentManager::loadEquipmentFromJson("equipment");
	pawnsList = PawnsManager::loadPawnsFromJson("pawns");
	iconSprites = initializeSpriteMap(iconTextures);
	equipmentFunctions = initializeFunctions();
	pawnFunctions = initializePawnFunctions();
	cell = initializeCells();
	cellHeight = 30;
	sumOfEqCellWidths = getSumOfArray(equipmentCellWidths);
	sumOfPnCellWidths = getSumOfArray(pawnCellWidths);
	backgroundSprite = loadBackgroundSprite();
	backgroundSprite.setPosition(0, 0);
	initializeText();
}

Armory::~Armory()
{
	for (Equipment* equipment : equipmentList) {
		delete equipment;
	}
}

void Armory::start() {
	closed = false;
	initializeEquipmentTable();
	initializePawnsTable();

	while (!closed && window->isOpen())
	{
		window->clear(sf::Color(71, 31, 16));
		window->draw(backgroundSprite);
		window->draw(pawnsTableSprite);
		window->display();

		sf::Event event;
		while (!closed && window->pollEvent(event)) {
			keyPressed(event);
		}
	}
}

void Armory::keyPressed(const sf::Event& event) {
	if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
		sf::Vector2i mousePosition = sf::Mouse::getPosition(*window);
		if (backClicked(mousePosition))
		{
			exit();
		}
	}
	else if (event.type == sf::Event::Closed)
	{
		window->close();
	}
}

bool Armory::backClicked(sf::Vector2i mousePosition)
{
	if (backCell.getGlobalBounds().contains(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y)))
	{
		return true;
	}
	return false;
}

void Armory::exit()
{
	closed = true;
}

void Armory::createEquipmentTexture()
{
	drawTitleText();
	drawHeaders('e');
	drawEquipment();
	drawBackButton();
}

void Armory::createPawnsTexture()
{
	drawTitleText();
	drawHeaders('p');
	drawPawns();
	drawBackButton();
}

void Armory::drawHeaders(char which)
{
	cell.setPosition(position);
	setPosSpriteMap(position.x + 3, position.y + 3, iconSprites);
	setScalSpriteMap(0.04, iconSprites);
	text.setPosition(position.x + 5, position.y + 3);
	cell.setFillColor(sf::Color(156, 84, 84));
	if (which == 'e')
	{
		drawEquipmentHeaders();
	}
	else if (which == 'p')
	{
		drawPawnsHeaders();
	}
	else
	{
		throw std::runtime_error("Invalid input");
	}
	
}

void Armory::drawTitleText()
{
	sf::Text newTitleText("Armory", globalFont2, fontSize * 1.5);
	newTitleText.setPosition(20, 10);
	newTitleText.setFillColor(sf::Color::White);
	titleText = newTitleText;
	tableRenderTexture.draw(titleText);
}

void Armory::drawEquipmentHeaders()
{
	for (int i = 0; i < 8; i++)
	{
		cell.setSize(sf::Vector2f(equipmentCellWidths[i], cellHeight));
		tableRenderTexture.draw(cell);
		if (equipmentHeaders[i].length() < 9)
		{
			text.setString(equipmentHeaders[i]);
			tableRenderTexture.draw(text);
		}
		else
		{
			tableRenderTexture.draw(iconSprites[equipmentHeaders[i]]);
		}
		cell.move(equipmentCellWidths[i], 0);
		text.move(equipmentCellWidths[i], 0);
		moveSpriteMap(equipmentCellWidths[i], 0, iconSprites);
	}
}

void Armory::drawPawnsHeaders()
{
	for (int i = 0; i < 6; i++)
	{
		cell.setSize(sf::Vector2f(pawnCellWidths[i], cellHeight));
		tableRenderTexture.draw(cell);
		if (pawnHeaders[i].length() < 9)
		{
			text.setString(pawnHeaders[i]);
			tableRenderTexture.draw(text);
		}
		else
		{
			tableRenderTexture.draw(iconSprites[pawnHeaders[i]]);
		}
		cell.move(pawnCellWidths[i], 0);
		text.move(pawnCellWidths[i], 0);
		moveSpriteMap(pawnCellWidths[i], 0, iconSprites);
	}
}

void Armory::drawEquipment()
{
	cell.move(-sumOfEqCellWidths, cellHeight);
	text.move(-sumOfEqCellWidths, cellHeight);
	moveSpriteMap(-sumOfEqCellWidths, cellHeight, iconSprites);

	for (Equipment* item : equipmentList)
	{
		cell.setFillColor(getTypeColor(item));
		for (int i = 0; i < 8; i++)
		{
			cell.setSize(sf::Vector2f(equipmentCellWidths[i], cellHeight));
			tableRenderTexture.draw(cell);
			if (i == 2)
			{
				drawSpaceIcon(item->getSpaceOccupied(), tableRenderTexture, iconSprites);
			}
			else if (i == 5)
			{
				drawTypeIcon(item->getType(), tableRenderTexture, iconSprites);
			}
			else
			{
				text.setString(equipmentFunctions[i](*static_cast<const Equipment*>(item)));
				tableRenderTexture.draw(text);
			}
			cell.move(equipmentCellWidths[i], 0);
			text.move(equipmentCellWidths[i], 0);
			moveSpriteMap(equipmentCellWidths[i], 0, iconSprites);
		}
		cell.move(-sumOfEqCellWidths, cellHeight);
		text.move(-sumOfEqCellWidths, cellHeight);
		moveSpriteMap(-sumOfEqCellWidths, cellHeight, iconSprites);
	}
}

void Armory::drawPawns()
{
	cell.move(-sumOfPnCellWidths, cellHeight);
	text.move(-sumOfPnCellWidths, cellHeight);
	moveSpriteMap(-sumOfPnCellWidths, cellHeight, iconSprites);

	for (Pawn* pawn : pawnsList)
	{
		cell.setFillColor(getTeamColor(pawn->getTeamNumber()));
		for (int i = 0; i < 6; i++)
		{
			cell.setSize(sf::Vector2f(pawnCellWidths[i], cellHeight));
			tableRenderTexture.draw(cell);

			text.setString(pawnFunctions[i](*static_cast<const Pawn*>(pawn)));
			tableRenderTexture.draw(text);

			cell.move(pawnCellWidths[i], 0);
			text.move(pawnCellWidths[i], 0);
			moveSpriteMap(pawnCellWidths[i], 0, iconSprites);
		}
		cell.move(-sumOfPnCellWidths, cellHeight);
		text.move(-sumOfPnCellWidths, cellHeight);
		moveSpriteMap(-sumOfPnCellWidths, cellHeight, iconSprites);
	}
}

sf::Color Armory::getTypeColor(Equipment* item)
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

sf::Color Armory::getTeamColor(int team)
{
	switch (team)
	{
	case 0:
		return sf::Color::Red;
	case 1:
		return sf::Color::Blue;
	case 2:
		return sf::Color::Green;
	case 3:
		return sf::Color::Yellow;
	case 4:
		return sf::Color::Magenta;
	case 5:
		return sf::Color::Cyan;
	default:
		return sf::Color::White;
	}
}

void Armory::drawBackButton()
{
	titleText.setString("back");
	sf::FloatRect textBounds = titleText.getLocalBounds();
	backCell.setSize(sf::Vector2f(textBounds.width*1.2, cellHeight*1.2));
	backCell.setPosition(window->getSize().x- backCell.getSize().x-20, 20);
	titleText.setPosition(window->getSize().x - backCell.getSize().x-15, 20);
	backCell.setFillColor(sf::Color(109, 201, 169));
	backCell.setOutlineColor(sf::Color::White);
	backCell.setOutlineThickness(1.f);
	tableRenderTexture.draw(backCell);
	tableRenderTexture.draw(titleText);
}

void Armory::initializeText()
{
	sf::Text newText;
	newText.setFont(globalFont2);
	newText.setCharacterSize(fontSize);
	newText.setFillColor(sf::Color::White);
	text = newText;
}

sf::Sprite Armory::loadBackgroundSprite()
{
	sf::Sprite newSprite;
	if (!backgroundTexture.loadFromFile("assets/backgrounds/armory.png")) {
		throw std::runtime_error("Unable to load background");
	}

	newSprite = sf::Sprite(backgroundTexture);
	return newSprite;
}

void Armory::initializeEquipmentTable()
{
	tableRenderTexture.clear(sf::Color::Transparent);
	createEquipmentTexture();
	tableRenderTexture.display();
	equipmentTableSprite = sf::Sprite(tableRenderTexture.getTexture());
}

void Armory::initializePawnsTable()
{
	tableRenderTexture.clear(sf::Color::Transparent);
	createPawnsTexture();
	tableRenderTexture.display();
	pawnsTableSprite = sf::Sprite(tableRenderTexture.getTexture());
}
