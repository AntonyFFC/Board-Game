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
// IF weapons contains onlt one weapon then skip the choosing table when called
WeaponsTable::WeaponsTable(Pawn* inPawn, Pawn* inAttacked, std::vector<Equipment*> weapons, sf::RenderWindow* inWindow)
    :attacker(inPawn), attacked(inAttacked), weapons(weapons), headers{"Name","left-right-arrow-icon","circle-line-icon","bomb-blast-icon",
 "history-icon","cube-icon","dollar-icon","Other" }, cellWidths{ 70,40,40,30,30,30,30,100 }
{
    sumOfCellWidths = getSumOfArray(cellWidths);
    target = inWindow;
    cellHeight = 20;
    gap = 5;
    setUpDimensions();
    iconSprites = initializeSpriteMap(iconTextures);
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
                drawSpaceIcon(item->getSpaceOccupied(),target, iconSprites);
            }
            else if (i == 5)
            {
                drawTypeIcon(item->getType(), target, iconSprites);
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

bool WeaponsTable::tableClicked(sf::Vector2i mousePosition)
{
    int clickMinY = minY + cellHeight;
    if (mousePosition.x >= minX && mousePosition.x <= maxX && mousePosition.y >= clickMinY && mousePosition.y <= maxY)
    {
        return true;
    }
    return false;
}

Pawn* WeaponsTable::getAttacker() const
{
    return attacker;
}

Pawn* WeaponsTable::getAttacked() const
{
    return attacked;
}

Equipment* WeaponsTable::getWeapon(sf::Vector2i mousePosition)
{
    return clickOnEquipment(mousePosition);
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