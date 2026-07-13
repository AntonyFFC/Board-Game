#include "Armory.h"

#include "ui/Layout.h"

template <typename T, size_t N>
int getSumOfArray(T(&arr)[N]) {
	int sum = 0;
	for (size_t i = 0; i < N; ++i) {
		sum += arr[i];
	}
	return sum;
}

Armory::Armory()
	: equipmentHeaders{ "Name","left-right-arrow-icon-white","circle-line-icon-white","bomb-blast-icon-white",
"history-icon-white","cube-icon-white","dollar-icon-white","Other" }, pawnHeaders{ "Name","history-icon-white",
"hand-line-icon-white","plus-round-line-icon-white","heart-line-icon-white","dollar-icon-white", "Other"},
equipmentCellWidths{ 150,60,60,50,50,50,50,500 }, pawnCellWidths{ 250, 50, 50,50, 50, 50, 250 },
backButton({}, { 200.f, 50.f }, "Back"),
changeButton({}, { 200.f, 50.f }, "-}")
{
	fontSize = 20;
	position = sf::Vector2f(20, 60);
	initializeFont();
	equipmentList = EquipmentManager::loadEquipmentFromJson("assets/equipment");
	pawnsList = PawnsManager::loadPawnsFromJson("assets/pawns");
	iconSprites = initializeSpriteMap(iconTextures);
	equipmentFunctions = initializeFunctions();
	pawnFunctions = initializePawnFunctions();
	cell = initializeCells();
	cellHeight = 30;
	sumOfEqCellWidths = getSumOfArray(equipmentCellWidths);
	sumOfPnCellWidths = getSumOfArray(pawnCellWidths);
	backgroundSprite = loadBackgroundSprite(&backgroundTexture, "armory");
	text = initializeText("Nothing", &globalFont2, fontSize, sf::Color::White);
	titleText = initializeText("Armory", &globalFont2, fontSize * 1.5, sf::Color::White);
}

Armory::~Armory()
{
	for (Equipment* equipment : equipmentList) {
		delete equipment;
	}
}

void Armory::rebuildLayout(const sf::FloatRect& root)
{
	root_ = root;
	tableArea_ = Layout::inset(root, root.width * 0.02f, root.height * 0.08f,
		root.width * 0.02f, root.height * 0.12f).toFloatRect();

	Layout::scaleSpriteToCover(backgroundSprite, root);

	const sf::FloatRect bottomBar = Layout::uiBar(root, false, 90.f).toFloatRect();
	const float barCenterY = bottomBar.top + bottomBar.height * 0.5f;
	changeButton.setPosition({
		root.left + root.width * 0.5f - 100.f,
		barCenterY - 25.f });
	backButton.setPosition({
		root.left + root.width - 220.f,
		root.top + root.height * 0.02f });

	const unsigned texW = static_cast<unsigned>(std::max(1.f, tableArea_.width));
	const unsigned texH = static_cast<unsigned>(std::max(1.f, tableArea_.height));
	equipmentRenderTexture.create(texW, texH);
	pawnsRenderTexture.create(texW, texH);
	position = sf::Vector2f(20.f, 60.f);

	if (!tablesBuilt_) {
		initializeEquipmentTable();
		initializePawnsTable();
		tablesBuilt_ = true;
	}

	equipmentTableSprite.setScale(
		tableArea_.width / static_cast<float>(texW),
		tableArea_.height / static_cast<float>(texH));
	equipmentTableSprite.setPosition(tableArea_.left, tableArea_.top);
	pawnsTableSprite.setScale(
		tableArea_.width / static_cast<float>(texW),
		tableArea_.height / static_cast<float>(texH));
	pawnsTableSprite.setPosition(tableArea_.left, tableArea_.top);
}

void Armory::handleEvent(const sf::Event& event, sf::Vector2f logicalMouse)
{
	if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
		if (backButton.isClicked(logicalMouse)) {
			wantsExit_ = true;
		}
		else if (changeButton.isClicked(logicalMouse)) {
			flipPage();
		}
	}
	else if (event.type == sf::Event::KeyPressed) {
		if (event.key.code == sf::Keyboard::Escape) {
			wantsExit_ = true;
		}
		else if (event.key.code == sf::Keyboard::Right || event.key.code == sf::Keyboard::Left) {
			flipPage();
		}
	}
}

void Armory::update(sf::Vector2f logicalMouse, sf::RenderWindow* cursorWindow)
{
	Button::updateAll(logicalMouse, cursorWindow);
}

void Armory::draw(sf::RenderTarget& target)
{
	target.draw(backgroundSprite);
	target.draw(isPawnsShown ? pawnsTableSprite : equipmentTableSprite);
	changeButton.setText(isPawnsShown ? "{-" : "-}");
	changeButton.draw(dynamic_cast<sf::RenderWindow&>(target));
	backButton.draw(dynamic_cast<sf::RenderWindow&>(target));
}

void Armory::createEquipmentTexture()
{
	drawTitleText('e');
	drawHeaders('e');
	drawEquipment();
}

void Armory::createPawnsTexture()
{
	drawTitleText('p');
	drawHeaders('p');
	drawPawns();
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

void Armory::drawTitleText(char which)
{
	titleText.setPosition(20, 10);
	if (which == 'e')
	{
		equipmentRenderTexture.draw(titleText);
	}
	else
	{
		pawnsRenderTexture.draw(titleText);
	}
}

void Armory::drawEquipmentHeaders()
{
	for (int i = 0; i < 8; i++)
	{
		cell.setSize(sf::Vector2f(equipmentCellWidths[i], cellHeight));
		equipmentRenderTexture.draw(cell);
		if (equipmentHeaders[i].length() < 9)
		{
			text.setString(equipmentHeaders[i]);
			equipmentRenderTexture.draw(text);
		}
		else
		{
			equipmentRenderTexture.draw(iconSprites[equipmentHeaders[i]]);
		}
		cell.move(equipmentCellWidths[i], 0);
		text.move(equipmentCellWidths[i], 0);
		moveSpriteMap(equipmentCellWidths[i], 0, iconSprites);
	}
}

void Armory::drawPawnsHeaders()
{
	for (int i = 0; i < 7; i++)
	{
		cell.setSize(sf::Vector2f(pawnCellWidths[i], cellHeight));
		pawnsRenderTexture.draw(cell);
		if (pawnHeaders[i].length() < 9)
		{
			text.setString(pawnHeaders[i]);
			pawnsRenderTexture.draw(text);
		}
		else
		{
			pawnsRenderTexture.draw(iconSprites[pawnHeaders[i]]);
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
			equipmentRenderTexture.draw(cell);
			if (i == 2)
			{
				drawSpaceIcon(item->getSpaceOccupied(), equipmentRenderTexture, iconSprites);
			}
			else if (i == 5)
			{
				drawTypeIcon(item->getType(), equipmentRenderTexture, iconSprites);
			}
			else
			{
				text.setString(equipmentFunctions[i](*static_cast<const Equipment*>(item)));
				equipmentRenderTexture.draw(text);
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
		for (int i = 0; i < 7; i++)
		{
			cell.setSize(sf::Vector2f(pawnCellWidths[i], cellHeight));
			pawnsRenderTexture.draw(cell);

			text.setString(pawnFunctions[i](*static_cast<const Pawn*>(pawn)));
			pawnsRenderTexture.draw(text);

			cell.move(pawnCellWidths[i], 0);
			text.move(pawnCellWidths[i], 0);
			moveSpriteMap(pawnCellWidths[i], 0, iconSprites);
		}
		cell.move(-sumOfPnCellWidths, cellHeight);
		text.move(-sumOfPnCellWidths, cellHeight);
		moveSpriteMap(-sumOfPnCellWidths, cellHeight, iconSprites);
	}
}

void Armory::initializeEquipmentTable()
{
	equipmentRenderTexture.clear(sf::Color::Transparent);
	createEquipmentTexture();
	equipmentRenderTexture.display();
	equipmentTableSprite = sf::Sprite(equipmentRenderTexture.getTexture());
}

void Armory::initializePawnsTable()
{
	pawnsRenderTexture.clear(sf::Color::Transparent);
	createPawnsTexture();
	pawnsRenderTexture.display();
	pawnsTableSprite = sf::Sprite(pawnsRenderTexture.getTexture());
}

void Armory::flipPage()
{
	isPawnsShown = !isPawnsShown;
}
