#include "TradeTable.h"


TradeTable::TradeTable(Pawn* inPawn, Pawn* inBody, sf::RenderWindow* inWindow)
	:pawn(inPawn), body(inBody), target(inWindow), leftTable(inWindow), rightTable(inWindow)
{
    gap = 10;
    initializeTables();
    setUpDimensions();
    doneCell = initializeCells();
    tradeText = initializeText("trade", &globalFont2, leftTable.cellHeight * 3 / 4, sf::Color::Black); // table height isnt calclulated yet
    tableRenderTexture.create(target->getSize().x, target->getSize().y);
    setUpButton();
    initializeTexture();
}

TradeTable::~TradeTable() = default;

void TradeTable::draw()
{
    target->draw(tableSprite);
}

void TradeTable::setUpButton()
{
    tradeText.setString("Done");
    sf::FloatRect textBounds = tradeText.getLocalBounds();
    tradeText.setFont(globalFont2);
    tradeText.setCharacterSize(20);
    tradeText.setFillColor(sf::Color::White);

    doneCell.setSize(sf::Vector2f(textBounds.width * 1.2, leftTable.cellHeight));
    doneCell.setFillColor(sf::Color(109, 201, 169));
}

void TradeTable::createTexture()
{
    setUpPositions();
    drawTables();
    drawDoneButton();
}

void TradeTable::setUpPositions()
{
    sf::FloatRect textBounds = tradeText.getLocalBounds();
    doneCell.setPosition(minX + leftTable.getSize().x - doneCell.getSize().x / 2 + gap / 2, minY - leftTable.cellHeight - gap);
    tradeText.setPosition(minX + leftTable.getSize().x - textBounds.width / 2 + gap / 2, minY - leftTable.cellHeight - gap);
}

void TradeTable::drawTables()
{
	tableRenderTexture.draw(leftTable.getTableSprite());
	tableRenderTexture.draw(rightTable.getTableSprite());
}

void TradeTable::drawDoneButton()
{
    tableRenderTexture.draw(doneCell);
    tableRenderTexture.draw(tradeText);
}

bool TradeTable::tableClicked(sf::Vector2i mousePosition)
{
    if (leftTable.tableClicked(mousePosition) || rightTable.tableClicked(mousePosition))
    {
        return true;
    }
    return false;
}

bool TradeTable::doneClicked(sf::Vector2i mousePosition)
{
    if (doneCell.getGlobalBounds().contains(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y)))
    {
        return true;
    }
    return false;
}

void TradeTable::trade(sf::Vector2i mousePosition)
{
    Equipment* item = nullptr;
    bool isBodys;
    if (rightTable.tableClicked(mousePosition))
    {
		item = rightTable.getClickedItem(mousePosition);
        isBodys = true;
    }
    else
    {
		item = leftTable.getClickedItem(mousePosition);
        isBodys = false;
    }
    if (item != nullptr)
    {
        tradeItem(item, isBodys);
        initializeTables();
        initializeTexture();
    }
}

void TradeTable::tradeItem(Equipment* item, bool isBodys)
{
    if (isBodys)
    {
        if (pawn->addEquipment(item))
        {
            body->removeEquipment(item);
            setUpDimensions();
        }
    }
    else
    {
        if (body->addEquipment(item))
        {
            pawn->removeEquipment(item);
            setUpDimensions();
        }
    }
}

void TradeTable::initializeTables()
{
	leftTable.setEquipment(pawn->getEquipment());
	rightTable.setEquipment(body->getEquipment());
	setUpDimensions();
    leftTable.setPosition(sf::Vector2f(minX, minY));
    rightTable.setPosition(sf::Vector2f(minX + leftTable.getSize().x + gap, minY));
	leftTable.createTexture();
	rightTable.createTexture();
}

void TradeTable::initializeTexture()
{
    tableRenderTexture.clear(sf::Color::Transparent);
    createTexture();
    tableRenderTexture.display();
    tableSprite = sf::Sprite(tableRenderTexture.getTexture());
}

void TradeTable::setUpDimensions()
{
    minX = target->getSize().x - leftTable.getSize().x * 2 - gap * 2;
    minY = 35;
    maxX = target->getSize().x - gap;
    maxY = minY + leftTable.cellHeight * (std::max(pawn->getEquipmentCount(), body->getEquipmentCount()) + 1);
}
