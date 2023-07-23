#include "Pawns.h"

Pawns::Pawns(Board* board)
    :board(board), cellSizes{ 70,40,40,30,30,30,30,100 }
{
    previous = empty;
    wasShift = false;
    isTrading_ = false;
    whosTurn = 0;
    whichPawn = 0;
    setupText();
    iconSprites = initializeSpriteMap();
    tableY = 35;
    cellHeight = 20;
}

template <typename T, size_t N>
int getSumOfArray(T(&arr)[N]) {
    int sum = 0;
    for (size_t i = 0; i < N; ++i) {
        sum += arr[i];
    }
    return sum;
}

void Pawns::flipTurn()
{
    if (whosTurn)
    {
        whosTurn =  0;
    }
    else
    {
        whosTurn = 1;
    }
    setupText();
}

int Pawns::numberOfPawn(std::tuple<int, int, int> coords, bool body)
{
    int keyValue = -1;
    Pawn* targetObject = nullptr;
    if (body) {
        targetObject = board->hexDict[coords]->body;
    }
    else {
        targetObject = board->hexDict[coords]->pawn;
    }

    for (const Pawn* pawn : pawnDict) {
        keyValue++;
        if (pawn == targetObject) {
            return keyValue;
        }
    }
}

void Pawns::handleClick(sf::Vector2i mousePosition, sf::RenderTarget& target)
{
    if (isTrading() && tableClicked(mousePosition, target))
    {
        std::cout << "Table clicked\n";
    }
    else
    {
        for (auto& pair : board->hexDict) {
            std::tuple<int, int, int> present = pair.first;
            if (board->hexDict[present]->isClicked(mousePosition)) {
                if ((!board->hexDict[present]->isPawn() || !board->hexDict[present]->pawn->isAlive()) && !board->hexDict[present]->isHigh(0))
                {
                    board->clearHighlight();
                    previous = empty;
                }
                else if (board->hexDict[present]->isPawn() && board->hexDict[present]->pawn->isAlive())
                {
                    if (previous != empty)
                    {
                        pawnSecond(whichPawn, present);
                    }
                    else
                    {
                        if (!pawnFirst(whichPawn, present))
                        {
                            break;
                        }
                    }
                }
                else
                {
                    highlightedNoPawn(whichPawn, present);
                }
                break;
            }
        }
    }
}

bool Pawns::tableClicked(sf::Vector2i mousePosition, sf::RenderTarget& target)
{
    int sumOfArr = getSumOfArray(cellSizes);
    int minX = target.getSize().x - sumOfArr * (2.04);
    int maxX = target.getSize().x - 0.02;
    int minY = tableY+ cellHeight;
    int maxY = minY + +cellHeight * 5;
    if (mousePosition.x >= minX && mousePosition.x <= maxX && mousePosition.y >= minY && mousePosition.y <= maxY)
    {
        return true;
    }
    return false;
}

void Pawns::pawnSecond(int pawnNum, std::tuple<int, int, int> current)
{
    std::vector<std::tuple<int, int, int>> inView = getViewOfPawn(pawnNum);
    auto it = std::find(inView.begin(), inView.end(), current);
    if (it != inView.end())
    {
        attack(pawnNum, numberOfPawn(current));
        board->clearHighlight();
        if (pawnDict[pawnNum]->getRemainingActions() == pawnDict[pawnNum]->getMaxActions())
        {
            previous = empty;
            flipTurn();
        }
        else
        {
            pawnClicked(pawnNum);
        }
    }
}

bool Pawns::pawnFirst(int pawnNum, std::tuple<int, int, int> current)
{
    if (whosTurn == board->hexDict[current]->pawn->getSide())
    {
        whichPawn = numberOfPawn(current);
        pawnClicked(whichPawn);
        previous = current;
        return true;
    }
    else {
        board->clearHighlight();
        previous = empty;
        return false; //stop the loop
    }
}

void Pawns::highlightedNoPawn(int pawnNum, std::tuple<int, int, int> current)
{
    pawnMoved(pawnNum, current);
    board->clearHighlight();
    if (pawnDict[pawnNum]->getRemainingActions() == pawnDict[pawnNum]->getMaxActions())
    {
        previous = empty;
        flipTurn();
    }
    else
    {
        pawnClicked(pawnNum);
        previous = current;
    }
}

void Pawns::handleClickRight(sf::Vector2i mousePosition)
{
    std::tuple<int, int, int> pawnCoords = pawnDict[whichPawn]->getHexCoords();
    if (board->hexDict[pawnCoords]->isClicked(mousePosition) && board->hexDict[pawnCoords]->hasBody())
    {
        board->clearHighlight();
        pawnDict[whichPawn]->reduceActions(1); //later you must simplify this, exchangging weapons must be added to leftClisk event when istrading is true adne set it to false
        setTrading(true);
        if (pawnDict[whichPawn]->getRemainingActions() == 0)
        {
            pawnDict[whichPawn]->setRemainingActions(pawnDict[whichPawn]->getMaxActions());
            previous = empty;
            flipTurn();
        }
        else
        {
            previous = empty;
        }
    }
    else
    {
        for (const std::tuple<int, int, int> coords : board->getNeighbours(pawnCoords)) {
            if (board->hexDict[coords]->isClicked(mousePosition)) {
                if (board->hexDict[coords]->isBlocking())
                {
                    if (!board->hexDict[coords]->isWall() || !destroyWall(whichPawn, coords))
                    {
                        board->clearHighlight();
                        previous = empty;
                    }
                }
                else
                {
                    placeWall(whichPawn, coords);
                }
                break;
            }
        }
    }
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

void Pawns::drawSpaceIcon(sf::RenderTarget& target, Equipment::SpaceOccupied space)
{
    int added = 0;
    for (int i = 0; i < space.numSpaces; i++)
    {
        if (space.spaceType == "hands")
        {
            target.draw(iconSprites["hand-line-icon"]);
            iconSprites["hand-line-icon"].move(15, 0);
        }
        else
        {
            target.draw(iconSprites["plus-round-line-icon"]);
            iconSprites["plus-round-line-icon"].move(15, 0);
        }
        added++;
    }
    if (space.spaceType == "hands")
    {
        iconSprites["hand-line-icon"].move(-15*added, 0);
    }
    else
    {
        iconSprites["plus-round-line-icon"].move(-15 * added, 0);
    }

}

void Pawns::drawTypeIcon(sf::RenderTarget& target, std::string type)
{
    if (type == "Weapon")
    {
        target.draw(iconSprites["crossed-swords-icon"]);
    }
    else if (type == "Armour")
    {
        target.draw(iconSprites["shield-icon"]);
    }
    else
    {
        target.draw(iconSprites["four-squares-icon"]);
    }
}

void setScalSpriteMap(float scl, std::map<std::string, sf::Sprite>& spriteMap)
{
    for (auto& pair : spriteMap)
    {
        sf::Sprite& currentSprite = pair.second;
        currentSprite.setScale(scl, scl);
    }
}

void setPosSpriteMap(int x, int y, std::map<std::string, sf::Sprite> &spriteMap)
{
    for (auto& pair : spriteMap)
    {
        sf::Sprite& currentSprite = pair.second;
        currentSprite.setPosition(x, y);
    }
}

void moveSpriteMap(int addx, int addy, std::map<std::string, sf::Sprite> &spriteMap)
{
    for (auto& pair : spriteMap)
    {
        sf::Sprite& currentSprite = pair.second;
        currentSprite.move(addx, addy);
    }
}

void Pawns::drawTable(sf::RenderTarget& target, std::vector<Equipment*> equipment, int index)
{
    int sumOfArr = getSumOfArray(cellSizes);
    std::string headers[] = { "Name","left-right-arrow-icon","circle-line-icon","bomb-blast-icon",
        "history-icon","cube-icon","dollar-icon","Other" };
    std::vector<std::function<std::string(const Equipment&)>> functions;
    functions.push_back([](const Equipment& item) { return item.getName(); });
    functions.push_back([](const Equipment& item) { return rangeToString(item.getRange()); });
    functions.push_back([](const Equipment& item) { return spaceToString(item.getSpaceOccupied()); }); //here draw the sprite with space
    functions.push_back([](const Equipment& item) { return std::to_string(item.getAttackValue()); });
    functions.push_back([](const Equipment& item) { return std::to_string(item.getAttackActions()); });
    functions.push_back([](const Equipment& item) { return item.getType(); }); //here draw the sprite with type
    functions.push_back([](const Equipment& item) { return std::to_string(item.getPrice()); });
    functions.push_back([](const Equipment& item) { return item.getAdditionalCapabilities(); });

    int innitialXpos = target.getSize().x - sumOfArr * (index*1.02);
    sf::RectangleShape cellOutline;
    cellOutline.setFillColor(sf::Color::Transparent);
    cellOutline.setOutlineColor(sf::Color::White);
    cellOutline.setFillColor(sf::Color(156, 84, 84));
    cellOutline.setOutlineThickness(1.f);
    cellOutline.setPosition(innitialXpos, tableY);
    sf::Text tradeText;
    int size = 15;
    tradeText.setFont(globalFont2);
    tradeText.setCharacterSize(size);
    tradeText.setFillColor(sf::Color::Black);
    tradeText.setPosition(innitialXpos, tableY);
    setPosSpriteMap(innitialXpos, tableY, iconSprites);
    setScalSpriteMap(0.04, iconSprites);

    for (int i = 0; i < 8; i++)
    {
        cellOutline.setSize(sf::Vector2f(cellSizes[i], cellHeight));
        target.draw(cellOutline);
        if (headers[i].substr(headers[i].length() - 4) != "icon")
        {
            tradeText.setString(headers[i]);
            target.draw(tradeText);
        }
        else
        {
            target.draw(iconSprites[headers[i]]);
        }
        cellOutline.move(cellSizes[i], 0);
        tradeText.move(cellSizes[i], 0);
        moveSpriteMap(cellSizes[i], 0, iconSprites);
    }
    cellOutline.setFillColor(sf::Color(200, 200, 200));
    cellOutline.move(-sumOfArr, cellHeight);
    tradeText.move(-sumOfArr, cellHeight);
    moveSpriteMap(-sumOfArr, cellHeight, iconSprites);

    for (Equipment* item : equipment)
    {
        for (int i = 0; i < 8; i++)
        {
            cellOutline.setSize(sf::Vector2f(cellSizes[i], cellHeight));
            target.draw(cellOutline);
            if (i == 2)
            {
                drawSpaceIcon(target, item->getSpaceOccupied());
            }
            else if (i == 5)
            {
                drawTypeIcon(target, item->getType());
            }
            else
            {
                tradeText.setString(functions[i](*static_cast<const Equipment*>(item)));
                target.draw(tradeText);
            }
            cellOutline.move(cellSizes[i], 0);
            tradeText.move(cellSizes[i], 0);
            moveSpriteMap(cellSizes[i], 0, iconSprites);
        }
        cellOutline.move(-sumOfArr, cellHeight);
        tradeText.move(-sumOfArr, cellHeight);
        moveSpriteMap(-sumOfArr, cellHeight, iconSprites);
    }
}

void Pawns::drawTrade(sf::RenderTarget& target)
{
    Pawn* pawn = pawnDict[whichPawn];
    Pawn* body = pawnDict[numberOfPawn(pawn->getHexCoords(), true)];
    std::vector<Equipment*> bodysEquipment = body->getEquipment();
    drawTable(target, bodysEquipment, 1);
    std::vector<Equipment*> pawnsEquipment = pawn->getEquipment();
    drawTable(target, pawnsEquipment, 2);
    
}

void Pawns::placeWall(int pawnNumber, std::tuple<int, int, int> coords)
{
    board->clearHighlight();
    pawnDict[pawnNumber]->reduceActions(1);
    board->setWall(coords);
    if (pawnDict[pawnNumber]->getRemainingActions() == 0)
    {
        pawnDict[pawnNumber]->setRemainingActions(pawnDict[pawnNumber]->getMaxActions());
        previous = empty;
        flipTurn();
    }
    else
    {
        pawnClicked(pawnNumber);
    }
}

bool Pawns::destroyWall(int pawnNumber, std::tuple<int, int, int> coords)
{
    board->clearHighlight();
    if (pawnDict[pawnNumber]->getRemainingActions() - 4 < 0)
    {
        return false;
    }
    else
    {
        board->setGrass(coords);
        pawnDict[pawnNumber]->reduceActions(4);
        if (pawnDict[pawnNumber]->getRemainingActions() == 0)
        {
            pawnDict[pawnNumber]->setRemainingActions(pawnDict[pawnNumber]->getMaxActions());
            previous = empty;
            flipTurn();
        }
        else
        {
            pawnClicked(pawnNumber);
        }
    }
    return true;
}

void Pawns::addPawn(Pawn* inPawn, std::tuple<int, int, int> coords)
{
    pawnDict.push_back(inPawn);
    board->hexDict[coords]->setPawn(true, inPawn);
}

void Pawns::handleShiftOn()
{
    if (!wasShift)
    {
        for (int i = 0; i<pawnDict.size(); i++)
        {
            std::vector<std::tuple<int, int, int>> inView = getViewOfPawn(i);
            board->highlighted[1].insert(board->highlighted[1].end(), inView.begin(), inView.end());
        }

        for (std::tuple<int, int, int> hex : board->highlighted[1])
        {
            if (board->hexDict[hex]->isHigh(1) || board->hexDict[hex]->isHigh(0))
            {
                board->hexDict[hex]->setHighlight(true, 2);
                continue;
            }
            board->hexDict[hex]->setHighlight(true, 1);
        }
    }
    wasShift = true;
}

void Pawns::handleShiftOff()
{
    for (std::tuple<int, int, int> hex : board->highlighted[1])
    {
        board->hexDict[hex]->setHighlight(false, 1);
        board->hexDict[hex]->setHighlight(false, 2);
    }
    board->highlighted[1].clear();
    wasShift = false;
}

bool Pawns::isTrading() const {
    return isTrading_;
}

bool Pawns::addItemToPawn(int number, Equipment* item)
{
    return pawnDict[number]->addEquipment(item);
}

void Pawns::setTrading(bool boolean) {
    isTrading_ = boolean;
}

void Pawns::drawTexts(sf::RenderTarget& target)
{
    drawTurn(target);
    if (isTrading())
    {
        drawTrade(target);
    }
}

void Pawns::drawTurn(sf::RenderTarget& target)
{
    target.draw(turnText);
}

void Pawns::pawnClicked(int pawnNum)
{
    board->highlighted[0] = getRangeOfPawn(pawnNum);
    for (std::tuple<int, int, int> hex : board->highlighted[0])
    {
        board->hexDict[hex]->setHighlight(true, 0);
    }
}

void Pawns::pawnMoved(int pawnNum, std::tuple<int, int, int> where)
{
    Pawn* pawn = pawnDict[pawnNum];
    pawn->reduceActions(board->hexDict[where]->getPawnDist());
    if (pawn->getRemainingActions() == 0)
    {
        pawn->setRemainingActions(pawn->getMaxActions());
    }
    board->hexDict[previous]->setPawn(false);
    board->hexDict[where]->setPawn(true, pawn);
}

void Pawns::attack(int pawnNum, int attackedNum)
{
    Pawn* attacker = pawnDict[pawnNum];
    Equipment* weapon = nullptr;
    for (Equipment* item : attacker->getEquipment()) //change this later for a function for choosing weapon
    {
        if (item->getType() == "Weapon")
        {
            weapon = item;
        }
    }
    if (weapon != nullptr)
    {
        Pawn* attacked = pawnDict[attackedNum];
        if (weapon->isRanged())
        {
            attacked->rangedAttack(weapon->getAttackValue(), weapon->getMissMax());
        }
        else
        {
            attacked->attack(weapon->getAttackValue());
        }
        if (!attacked->isAlive())
        {
            board->hexDict[attacked->getHexCoords()]->setBody(true, attacked);
        }
        attacker->reduceActions(weapon->getAttackActions());
        if (attacker->getRemainingActions() == 0)
        {
            attacker->setRemainingActions(attacker->getMaxActions());
        }
    }
    else {
        throw std::runtime_error("No weapon to attack with");
    }
}

std::vector<std::tuple<int, int, int>> Pawns::getViewOfWeapon(int pawnNum, Equipment* weapon)
{
    Pawn* pawn = pawnDict[pawnNum];
    Equipment::Range range = weapon->getRange();
    return board->getInView(pawn->getHexCoords(), range.maxRange, range.minRange);
}

std::vector<std::tuple<int, int, int>> Pawns::getViewOfPawn(int pawnNum)
{
    Pawn* pawn = pawnDict[pawnNum];
    Equipment* weapon = nullptr;
    std::vector<std::tuple<int, int, int>> inView;
    std::vector<std::tuple<int, int, int>> inViewWeapon;
    for (Equipment* item : pawn->getEquipment())
    {
        if (item->getType() == "Weapon")
        {
            weapon = item;
            inViewWeapon = getViewOfWeapon(pawnNum, weapon);
            inView.insert(inView.end(), inViewWeapon.begin(), inViewWeapon.end());
        }
    }
    return inView;
}

std::vector<std::tuple<int, int, int>> Pawns::getRangeOfPawn(int pawnNum)
{
    Pawn* pawn = pawnDict[pawnNum];
    std::vector<std::tuple<int, int, int>> inRange;
    inRange = board->getReachable(pawn->getHexCoords(), pawn->getRemainingActions());
    return inRange;
}

void Pawns::setupText()
{
    int size = 30;
    turnText.setFont(globalFont2);
    turnText.setCharacterSize(size);
    turnText.setPosition(10,10);
    if (whosTurn)
    {
        turnText.setFillColor(sf::Color::Blue);
        turnText.setString("Turn: Blue");
    }
    else
    {
        turnText.setFillColor(sf::Color::Red);
        turnText.setString("Turn: Red");
    }
}

std::map<std::string, sf::Sprite> Pawns::initializeSpriteMap()
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
