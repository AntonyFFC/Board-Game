#include "TradeTable.h"
#include "SpriteUtils.h"

template <typename T, size_t N>
int getSumOfArray(T(&arr)[N]) {
	int sum = 0;
	for (size_t i = 0; i < N; ++i) {
		sum += arr[i];
	}
	return sum;
}

TradeTable::TradeTable(Pawn* inPawn, Pawn* inBody, sf::RenderWindow* inWindow)
    :pawn(inPawn), body(inBody), target(inWindow), headers{ "Name","left-right-arrow-icon","circle-line-icon","bomb-blast-icon",
 "history-icon","cube-icon","dollar-icon","Other" }, cellWidths{ 70,40,40,30,30,30,30,100 }
{
	sumOfCellWidths = getSumOfArray(cellWidths);
    cellHeight = 20;
    gap = 5;
    setUpDimensions();
	iconSprites = initializeSpriteMap(iconTextures);
    functions = initializeFunctions();
    cell = initializeCells();
    doneCell = initializeCells();
    tradeText = initializeText("trade", &globalFont2, cellHeight*3/4, sf::Color::Black);
    tableRenderTexture.create(target->getSize().x, target->getSize().y);
    initializeTexture();
}

void TradeTable::draw()
{
    target->draw(tableSprite);
}

void TradeTable::createTexture(std::vector<Equipment*> equipment, bool bodys)
{
    setUpPositions(bodys);
    drawHeaders();
    drawEquipment(equipment);
    drawDoneButton();
}

void TradeTable::setUpPositions(bool bodys)
{
    cell.setPosition(minX, minY);
    setPosSpriteMap(minX, minY, iconSprites);
    setScalSpriteMap(0.04, iconSprites);
    tradeText.setPosition(minX, minY);
    if (bodys)
    {
        int xmove = sumOfCellWidths + gap;
        cell.move(xmove, 0);
        tradeText.move(xmove, 0);
        moveSpriteMap(xmove, 0, iconSprites);
    }
}

void TradeTable::drawHeaders()
{
    cell.setFillColor(sf::Color(156, 84, 84));

    for (int i = 0; i < 8; i++)
    {
        cell.setSize(sf::Vector2f(cellWidths[i], cellHeight));
        tableRenderTexture.draw(cell);
        if (headers[i].substr(headers[i].length() - 4) != "icon")
        {
            tradeText.setString(headers[i]);
            tableRenderTexture.draw(tradeText);
        }
        else
        {
            tableRenderTexture.draw(iconSprites[headers[i]]);
        }
        cell.move(cellWidths[i], 0);
        tradeText.move(cellWidths[i], 0);
        moveSpriteMap(cellWidths[i], 0, iconSprites);
    }
}

void TradeTable::drawEquipment(std::vector<Equipment*> equipment)
{
    cell.setFillColor(sf::Color(200, 200, 200));
    cell.move(-sumOfCellWidths, cellHeight);
    tradeText.move(-sumOfCellWidths, cellHeight);
    moveSpriteMap(-sumOfCellWidths, cellHeight, iconSprites);

    for (Equipment* item : equipment)
    {
        for (int i = 0; i < 8; i++)
        {
            cell.setSize(sf::Vector2f(cellWidths[i], cellHeight));
            tableRenderTexture.draw(cell);
            if (i == 2)
            {
                drawSpaceIconBlack(item->getSpaceOccupied(), tableRenderTexture, iconSprites);
            }
            else if (i == 5)
            {
                drawTypeIconBlack(item->getType(), tableRenderTexture, iconSprites);
            }
            else
            {
                tradeText.setString(functions[i](*static_cast<const Equipment*>(item)));
                tableRenderTexture.draw(tradeText);
            }
            cell.move(cellWidths[i], 0);
            tradeText.move(cellWidths[i], 0);
            moveSpriteMap(cellWidths[i], 0, iconSprites);
        }
        cell.move(-sumOfCellWidths, cellHeight);
        tradeText.move(-sumOfCellWidths, cellHeight);
        moveSpriteMap(-sumOfCellWidths, cellHeight, iconSprites);
    }
}

void TradeTable::drawDoneButton()
{
    tradeText.setString("Done");
    sf::FloatRect textBounds = tradeText.getLocalBounds();
    doneCell.setSize(sf::Vector2f(textBounds.width*1.2, cellHeight));
    doneCell.setPosition(minX+ sumOfCellWidths- doneCell.getSize().x/2+gap/2, minY - cellHeight - gap);
    tradeText.setPosition(minX + sumOfCellWidths - textBounds.width / 2 + gap / 2, minY - cellHeight - gap);
    doneCell.setFillColor(sf::Color(109, 201, 169));
    tableRenderTexture.draw(doneCell);
    tableRenderTexture.draw(tradeText);
}

bool TradeTable::tableClicked(sf::Vector2i mousePosition)
{
    int clickMinY = minY + cellHeight;
    if (mousePosition.x >= minX && mousePosition.x <= maxX && mousePosition.y >= clickMinY && mousePosition.y <= maxY)
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
    if (isOnBody(mousePosition))
    {
        item = clickOnEquipment(mousePosition, body->getEquipment());
        isBodys = true;
    }
    else
    {
        item = clickOnEquipment(mousePosition, pawn->getEquipment());
        isBodys = false;
    }
    if (item != nullptr)
    {
        tradeItem(item, isBodys);
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

Equipment* TradeTable::clickOnEquipment(sf::Vector2i mousePosition, std::vector<Equipment*> equipment)
{
    int itemMinY = minY;
    int itemMaxY = minY+ cellHeight;
    Equipment* chosen = nullptr;
    for (Equipment* item : equipment)
    {
        itemMinY += cellHeight;
        itemMaxY += cellHeight;
        if (mousePosition.y >= itemMinY && mousePosition.y <= itemMaxY)
        {
            chosen = item;
            break;
        }
    }
    return chosen;
}

bool TradeTable::isOnBody(sf::Vector2i mousePosition)
{
    int minXbody = minX + sumOfCellWidths * (1.02);
    if (mousePosition.x >= minXbody && mousePosition.x <= maxX)
    {
        return true;
    }
    return false;
}

void TradeTable::initializeTexture()
{
    std::vector<Equipment*> bodysEquipment = body->getEquipment();
    std::vector<Equipment*> pawnsEquipment = pawn->getEquipment();

    tableRenderTexture.clear(sf::Color::Transparent);
    createTexture(bodysEquipment, true);
    createTexture(pawnsEquipment, false);
    tableRenderTexture.display();
    tableSprite = sf::Sprite(tableRenderTexture.getTexture());
}

void TradeTable::setUpDimensions()
{
    minX = target->getSize().x - sumOfCellWidths * 2 - gap * 2;
    minY = 35;
    maxX = target->getSize().x - gap;
    maxY = minY + cellHeight * (std::max(pawn->getEquipmentCount(), body->getEquipmentCount()) + 1);
}