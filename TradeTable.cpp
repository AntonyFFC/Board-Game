#include "TradeTable.h"

template <typename T, size_t N>
int getSumOfArray(T(&arr)[N]) {
	int sum = 0;
	for (size_t i = 0; i < N; ++i) {
		sum += arr[i];
	}
	return sum;
}

void setScalSpriteMap(float scl, std::map<std::string, sf::Sprite>& spriteMap)
{
    for (auto& pair : spriteMap)
    {
        sf::Sprite& currentSprite = pair.second;
        currentSprite.setScale(scl, scl);
    }
}

void setPosSpriteMap(int x, int y, std::map<std::string, sf::Sprite>& spriteMap)
{
    for (auto& pair : spriteMap)
    {
        sf::Sprite& currentSprite = pair.second;
        currentSprite.setPosition(x, y);
    }
}

void moveSpriteMap(int addx, int addy, std::map<std::string, sf::Sprite>& spriteMap)
{
    for (auto& pair : spriteMap)
    {
        sf::Sprite& currentSprite = pair.second;
        currentSprite.move(addx, addy);
    }
}

TradeTable::TradeTable(Pawn* inPawn, Pawn* inBody, sf::RenderWindow* inWindow)
    :pawn(inPawn), body(inBody), headers{ "Name","left-right-arrow-icon","circle-line-icon","bomb-blast-icon",
 "history-icon","cube-icon","dollar-icon","Other" }, cellWidths{ 70,40,40,30,30,30,30,100 }
{
	sumOfCellWidths = getSumOfArray(cellWidths);
	target = inWindow;
    cellHeight = 20;
	minX = target->getSize().x - sumOfCellWidths * (2.04);
	minY = 35;
	maxX = target->getSize().x - 0.02;
	maxY = minY + cellHeight * (std::max(pawn->getEquipmentCount(), body->getEquipmentCount())+1);
	cellHeight = 20;
	iconSprites = initializeSpriteMap();
    functions = initializeFunctions();
    cell = initializeCells();
    tradeText = initializeTradeText();
}

void TradeTable::draw()
{
    std::vector<Equipment*> bodysEquipment = body->getEquipment();
    drawTable(bodysEquipment, true);
    std::vector<Equipment*> pawnsEquipment = pawn->getEquipment();
    drawTable(pawnsEquipment, false);

}

void TradeTable::drawTable(std::vector<Equipment*> equipment, bool bodys)
{
    cell.setPosition(minX, minY);
    setPosSpriteMap(minX, minY, iconSprites);
    setScalSpriteMap(0.04, iconSprites);
    tradeText.setPosition(minX, minY);
    if (bodys)
    {
        int xmove = sumOfCellWidths * (1.02);
        cell.move(xmove, 0);
        tradeText.move(xmove, 0);
        moveSpriteMap(xmove, 0, iconSprites);
    }
    cell.setFillColor(sf::Color(156, 84, 84));

    for (int i = 0; i < 8; i++)
    {
        cell.setSize(sf::Vector2f(cellWidths[i], cellHeight));
        target->draw(cell);
        if (headers[i].substr(headers[i].length() - 4) != "icon")
        {
            tradeText.setString(headers[i]);
            target->draw(tradeText);
        }
        else
        {
            target->draw(iconSprites[headers[i]]);
        }
        cell.move(cellWidths[i], 0);
        tradeText.move(cellWidths[i], 0);
        moveSpriteMap(cellWidths[i], 0, iconSprites);
    }
    cell.setFillColor(sf::Color(200, 200, 200));
    cell.move(-sumOfCellWidths, cellHeight);
    tradeText.move(-sumOfCellWidths, cellHeight);
    moveSpriteMap(-sumOfCellWidths, cellHeight, iconSprites);

    for (Equipment* item : equipment)
    {
        for (int i = 0; i < 8; i++)
        {
            cell.setSize(sf::Vector2f(cellWidths[i], cellHeight));
            target->draw(cell);
            if (i == 2)
            {
                drawSpaceIcon(item->getSpaceOccupied());
            }
            else if (i == 5)
            {
                drawTypeIcon(item->getType());
            }
            else
            {
                tradeText.setString(functions[i](*static_cast<const Equipment*>(item)));
                target->draw(tradeText);
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

void TradeTable::drawSpaceIcon(Equipment::SpaceOccupied space)
{
    int added = 0;
    for (int i = 0; i < space.numSpaces; i++)
    {
        if (space.spaceType == "hands")
        {
            target->draw(iconSprites["hand-line-icon"]);
            iconSprites["hand-line-icon"].move(15, 0);
        }
        else
        {
            target->draw(iconSprites["plus-round-line-icon"]);
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

void TradeTable::drawTypeIcon(std::string type)
{
    if (type == "Weapon")
    {
        target->draw(iconSprites["crossed-swords-icon"]);
    }
    else if (type == "Armour")
    {
        target->draw(iconSprites["shield-icon"]);
    }
    else
    {
        target->draw(iconSprites["four-squares-icon"]);
    }
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

void TradeTable::trade(sf::Vector2i mousePosition)
{
    // trading done here
}

// initializers
std::map<std::string, sf::Sprite> TradeTable::initializeSpriteMap()
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

std::string rangeToString(Equipment::Range range)
{
    std::string outText;
    outText = std::to_string(range.minRange) + "-" + std::to_string(range.maxRange);
    return outText;
}

std::string spaceToString(Equipment::SpaceOccupied space)
{
    std::string outText;
    outText = std::to_string(space.numSpaces) + ":" + space.spaceType;
    return outText;
}

std::vector<std::function<std::string(const Equipment&)>> TradeTable::initializeFunctions()
{
    std::vector<std::function<std::string(const Equipment&)>> newFunctions;
    newFunctions.push_back([](const Equipment& item) { return item.getName(); });
    newFunctions.push_back([](const Equipment& item) { return rangeToString(item.getRange()); });
    newFunctions.push_back([](const Equipment& item) { return spaceToString(item.getSpaceOccupied()); }); //here draw the sprite with space
    newFunctions.push_back([](const Equipment& item) { return std::to_string(item.getAttackValue()); });
    newFunctions.push_back([](const Equipment& item) { return std::to_string(item.getAttackActions()); });
    newFunctions.push_back([](const Equipment& item) { return item.getType(); }); //here draw the sprite with type
    newFunctions.push_back([](const Equipment& item) { return std::to_string(item.getPrice()); });
    newFunctions.push_back([](const Equipment& item) { return item.getAdditionalCapabilities(); });
    return newFunctions;
}

sf::RectangleShape TradeTable::initializeCells()
{
    sf::RectangleShape newCell;
    newCell.setFillColor(sf::Color::Transparent);
    newCell.setOutlineColor(sf::Color::White);
    newCell.setOutlineThickness(1.f);
    return newCell;
}

sf::Text TradeTable::initializeTradeText()
{
    sf::Text text;
    int size = 15;
    text.setFont(globalFont2);
    text.setCharacterSize(size);
    text.setFillColor(sf::Color::Black);
    return text;
}