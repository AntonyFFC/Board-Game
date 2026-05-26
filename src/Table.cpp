#include "Table.h"

template <typename T, size_t N>
int getSumOfArray(T(&arr)[N]) {
    int sum = 0;
    for (size_t i = 0; i < N; ++i) {
        sum += arr[i];
    }
    return sum;
}

Table::Table(sf::RenderWindow* inWindow)
	: target(inWindow), headers{ "Name","left-right-arrow-icon","circle-line-icon","bomb-blast-icon",
	 "history-icon","cube-icon","dollar-icon","Other" }, cellWidths{ 70,40,40,30,30,30,30,100 }
{
	sumOfCellWidths = getSumOfArray(cellWidths);
	cellHeight = 20;
	iconSprites = initializeSpriteMap(iconTextures);
	functions = initializeFunctions();
	cell = initializeCells();
	text = initializeText("Weapons", &globalFont2, cellHeight * 3 / 4, sf::Color::Black);
	tableRenderTexture.create(target->getSize().x, target->getSize().y);
}

Table::~Table() = default;

void Table::draw()
{
    target->draw(tableSprite);
}

bool Table::tableClicked(sf::Vector2i mousePosition)
{
    int clickMinY = minY + cellHeight;
    if (mousePosition.x >= minX && mousePosition.x <= maxX && mousePosition.y >= clickMinY && mousePosition.y <= maxY)
    {
        return true;
    }
    return false;
}

void Table::setPosition(sf::Vector2f inPosition)
{
	position = inPosition;
	setUpDimensions();
}

void Table::setEquipment(std::vector<Equipment*> inEquipment)
{
	equipment = inEquipment;
	numberOfItems = equipment.size();
	setUpDimensions();
}

sf::Vector2f Table::getPosition() const
{
	return position;
}

sf::Vector2f Table::getSize() const
{
    return sf::Vector2f(maxX - minX, maxY - minY);
}

Equipment* Table::getClickedItem(sf::Vector2i mousePosition)
{
    return clickOnEquipment(mousePosition);
}

void Table::createTexture()
{
    tableRenderTexture.clear(sf::Color::Transparent);
	drawHeaders();
	drawEquipment();
    tableRenderTexture.display();
    tableSprite = sf::Sprite(tableRenderTexture.getTexture());
}

sf::Sprite Table::getTableSprite()
{
	return tableSprite;
}

void Table::toggleHighlightClickeditem(sf::Vector2i mousePosition)
{
    int clickMinY = minY + cellHeight;
    int clickMaxY = minY + cellHeight * (numberOfItems+1);
    if (mousePosition.y >= clickMinY && mousePosition.y <= clickMaxY)
    {
        int clickedRow = (mousePosition.y - clickMinY) / cellHeight;
        if (clickedRow >= 0 && clickedRow < numberOfItems)
        {
            auto it = std::find(highlightedEquipmentIndices.begin(), highlightedEquipmentIndices.end(), clickedRow);
            if (it != highlightedEquipmentIndices.end())
            {
                highlightedEquipmentIndices.erase(it);
            }
            else
            {
                highlightedEquipmentIndices.push_back(clickedRow);
            }
            return;
        }
    }
}

std::vector<Equipment*> Table::getHighlightedItems() const
{
	std::vector<Equipment*> highlightedItems;
	for (int index : highlightedEquipmentIndices)
	{
		if (index >= 0 && index < numberOfItems)
		{
			highlightedItems.push_back(equipment[index]);
		}
	}
	return highlightedItems;
}

void Table::unhighlightAllItems()
{
	highlightedEquipmentIndices.clear();
}

void Table::drawHeaders()
{
    cell.setPosition(minX, minY);
    setPosSpriteMap(minX, minY, iconSprites);
    setScalSpriteMap(0.04, iconSprites);
    text.setPosition(minX, minY);
    cell.setFillColor(sf::Color(156, 84, 84));

    for (int i = 0; i < 8; i++)
    {
        cell.setSize(sf::Vector2f(cellWidths[i], cellHeight));
        tableRenderTexture.draw(cell);
        if (headers[i].substr(headers[i].length() - 4) != "icon")
        {
            text.setString(headers[i]);
            tableRenderTexture.draw(text);
        }
        else
        {
            tableRenderTexture.draw(iconSprites[headers[i]]);
        }
        cell.move(cellWidths[i], 0);
        text.move(cellWidths[i], 0);
        moveSpriteMap(cellWidths[i], 0, iconSprites);
    }
}

void Table::drawEquipment()
{
    cell.move(-sumOfCellWidths, cellHeight);
    text.move(-sumOfCellWidths, cellHeight);
    moveSpriteMap(-sumOfCellWidths, cellHeight, iconSprites);

    for (int i = 0; i < numberOfItems; ++i)
    {
        Equipment* item = equipment[i];

        if (std::find(highlightedEquipmentIndices.begin(), highlightedEquipmentIndices.end(), i) != highlightedEquipmentIndices.end())
        {
            cell.setFillColor(sf::Color(255, 255, 0));
        }
        else
        {
            cell.setFillColor(sf::Color(200, 200, 200));
        }

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
                text.setString(functions[i](*static_cast<const Equipment*>(item)));
                tableRenderTexture.draw(text);
            }
            cell.move(cellWidths[i], 0);
            text.move(cellWidths[i], 0);
            moveSpriteMap(cellWidths[i], 0, iconSprites);
        }
        cell.move(-sumOfCellWidths, cellHeight);
        text.move(-sumOfCellWidths, cellHeight);
        moveSpriteMap(-sumOfCellWidths, cellHeight, iconSprites);
    }
}

void Table::setUpDimensions()
{
    minX = position.x;
    minY = position.y;
    maxX = position.x+sumOfCellWidths;
    maxY = minY + cellHeight * (numberOfItems+1);
}

Equipment* Table::clickOnEquipment(sf::Vector2i mousePosition)
{
    int itemMinY = minY;
    int itemMaxY = minY + cellHeight;
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
