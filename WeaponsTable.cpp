#include "WeaponsTable.h"
#include "SpriteUtils.h"

template <typename T, size_t N>
int getSumOfArray(T(&arr)[N]) {
    int sum = 0;
    for (size_t i = 0; i < N; ++i) {
        sum += arr[i];
    }
    return sum;
}

std::vector<Equipment*> findWeapons(std::vector<Equipment*> equipment)
{
    std::vector<Equipment*> weapons;
    for (Equipment* item : equipment)
    {
        if (item->getType() == "Weapon")
        {
            weapons.push_back(item);
        }
    }
    return weapons;
}

WeaponsTable::WeaponsTable(Pawn* inPawn, Pawn* inAttacked, sf::RenderWindow* inWindow)
    :attacker(inPawn), attacked(inAttacked),headers{ "Name","left-right-arrow-icon","circle-line-icon","bomb-blast-icon",
 "history-icon","cube-icon","dollar-icon","Other" }, cellWidths{ 70,40,40,30,30,30,30,100 }
{
    weapons = findWeapons(inPawn->getEquipment());
    sumOfCellWidths = getSumOfArray(cellWidths);
    target = inWindow;
    cellHeight = 20;
    gap = 5;
    setUpDimensions();
    iconSprites = initializeSpriteMap();
    functions = initializeFunctions();
    cell = initializeCells();
    text = initializeText();
}

void WeaponsTable::draw()
{
    cell.setPosition(minX, minY);
    setPosSpriteMap(minX, minY, iconSprites);
    setScalSpriteMap(0.04, iconSprites);
    text.setPosition(minX, minY);
    cell.setFillColor(sf::Color(156, 84, 84));

    for (int i = 0; i < 8; i++)
    {
        cell.setSize(sf::Vector2f(cellWidths[i], cellHeight));
        target->draw(cell);
        if (headers[i].substr(headers[i].length() - 4) != "icon")
        {
            text.setString(headers[i]);
            target->draw(text);
        }
        else
        {
            target->draw(iconSprites[headers[i]]);
        }
        cell.move(cellWidths[i], 0);
        text.move(cellWidths[i], 0);
        moveSpriteMap(cellWidths[i], 0, iconSprites);
    }
    cell.setFillColor(sf::Color(200, 200, 200));
    cell.move(-sumOfCellWidths, cellHeight);
    text.move(-sumOfCellWidths, cellHeight);
    moveSpriteMap(-sumOfCellWidths, cellHeight, iconSprites);

    for (Equipment* item : weapons)
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
                text.setString(functions[i](*static_cast<const Equipment*>(item)));
                target->draw(text);
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

void WeaponsTable::drawSpaceIcon(Equipment::SpaceOccupied space)
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

void WeaponsTable::drawTypeIcon(std::string type)
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

bool WeaponsTable::tableClicked(sf::Vector2i mousePosition)
{
    int clickMinY = minY + cellHeight;
    if (mousePosition.x >= minX && mousePosition.x <= maxX && mousePosition.y >= clickMinY && mousePosition.y <= maxY)
    {
        return true;
    }
    return false;
}

Equipment* WeaponsTable::clickOnEquipment(sf::Vector2i mousePosition)
{
    int itemMinY = minY;
    int itemMaxY = minY + cellHeight;
    Equipment* chosen = nullptr;
    for (Equipment* item : weapons)
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

// initializers
std::map<std::string, sf::Sprite> WeaponsTable::initializeSpriteMap()
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

std::vector<std::function<std::string(const Equipment&)>> WeaponsTable::initializeFunctions()
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

sf::RectangleShape WeaponsTable::initializeCells()
{
    sf::RectangleShape newCell;
    newCell.setFillColor(sf::Color::Transparent);
    newCell.setOutlineColor(sf::Color::White);
    newCell.setOutlineThickness(1.f);
    return newCell;
}

sf::Text WeaponsTable::initializeText()
{
    sf::Text text;
    int size = 15;
    text.setFont(globalFont2);
    text.setCharacterSize(size);
    text.setFillColor(sf::Color::Black);
    return text;
}

void WeaponsTable::setUpDimensions()
{
    minX = target->getSize().x - sumOfCellWidths - gap;
    minY = 35;
    maxX = target->getSize().x - gap;
    maxY = minY + cellHeight * (weapons.size() + 1);
}