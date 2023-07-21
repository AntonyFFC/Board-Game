#include "Pawns.h"

Pawns::Pawns(Board* board)
    :board(board)
{
    previous = empty;
    wasShift = false;
    isTrading_ = false;
    whosTurn = 0;
    whichPawn = 0;
    setupText();
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

void Pawns::handleClick(sf::Vector2i mousePosition)
{
    for (auto& pair : board->hexDict) {
        std::tuple<int, int, int> present = pair.first;
        if (board->hexDict[present]->isClicked(mousePosition)) {
            if ((!board->hexDict[present]->isPawn()|| !board->hexDict[present]->pawn->isAlive()) && !board->hexDict[present]->isHigh(0))
            {
                board->clearHighlight();
                previous = empty;
            }
            else if (board->hexDict[present]->isPawn() && board->hexDict[present]->pawn->isAlive())
            {
                if (previous != empty) //this checks if this is not the first click of a player
                {
                    std::vector<std::tuple<int, int, int>> inView = getViewOfPawn(whichPawn);
                    auto it = std::find(inView.begin(), inView.end(), present);
                    if (it != inView.end())
                    {
                        attack(whichPawn, numberOfPawn(present));
                        board->clearHighlight();
                        if (pawnDict[whichPawn]->getRemainingActions() == pawnDict[whichPawn]->getMaxActions())
                        {
                            previous = empty;
                            flipTurn();
                        }
                        else
                        {
                            pawnClicked(whichPawn);
                        }
                    }
                }
                else //this occurs when it is the first click of the player
                {
                    if (whosTurn == board->hexDict[present]->pawn->getSide())
                    {
                        whichPawn = numberOfPawn(present);
                        pawnClicked(whichPawn);
                        previous = present;
                    }
                    else {
                        board->clearHighlight();
                        previous = empty;
                        break;
                    }
                }
            }
            else //this occurs when the hex is highlighted
            {
                pawnMoved(whichPawn, present);
                board->clearHighlight();
                if (pawnDict[whichPawn]->getRemainingActions() == pawnDict[whichPawn]->getMaxActions())
                {
                    previous = empty;
                    flipTurn();
                }
                else
                {
                    pawnClicked(whichPawn);
                    previous = present;
                }
            }
            break;
        }
    }
}

void Pawns::handleClickRight(sf::Vector2i mousePosition)
{
    std::tuple<int, int, int> pawnCoords = pawnDict[whichPawn]->getHexCoords();
    if (board->hexDict[pawnCoords]->isClicked(mousePosition))
    {
        board->clearHighlight();
        pawnDict[whichPawn]->reduceActions(1); //later you must simplify this, exchangging weapons must be added to leftClisk event when istrading is true
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

template <typename T, size_t N>
int getSumOfArray(T(&arr)[N]) {
    int sum = 0;
    for (size_t i = 0; i < N; ++i) {
        sum += arr[i];
    }
    return sum;
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

void drawTable(sf::RenderTarget& target, std::vector<Equipment*> bodysEquipment)
{
    int cellSizes[] = { 70,50,50,50,50,80,50,100 };
    int sumOfArr = getSumOfArray(cellSizes);
    std::string headers[] = { "Name","Range","Space","Attack","Actions","Type","Price","Other" };
    std::vector<std::function<std::string(const Equipment&)>> functions;
    functions.push_back([](const Equipment& item) { return item.getName(); });
    functions.push_back([](const Equipment& item) { return rangeToString(item.getRange()); });
    functions.push_back([](const Equipment& item) { return spaceToString(item.getSpaceOccupied()); });
    functions.push_back([](const Equipment& item) { return std::to_string(item.getAttackValue()); });
    functions.push_back([](const Equipment& item) { return std::to_string(item.getAttackActions()); });
    functions.push_back([](const Equipment& item) { return item.getType(); });
    functions.push_back([](const Equipment& item) { return std::to_string(item.getPrice()); });
    functions.push_back([](const Equipment& item) { return item.getAdditionalCapabilities(); });

    sf::RectangleShape cellOutline;
    cellOutline.setFillColor(sf::Color::Transparent);
    cellOutline.setOutlineColor(sf::Color::White);
    cellOutline.setFillColor(sf::Color(99, 51, 51));
    cellOutline.setOutlineThickness(1.f);
    cellOutline.setPosition(target.getSize().x - 550, 35);
    sf::Text tradeText;
    int size = 13;
    tradeText.setFont(globalFont2);
    tradeText.setCharacterSize(size * 0.7);
    tradeText.setFillColor(sf::Color::White);
    tradeText.setPosition(target.getSize().x - 550, 35);

    for (int i = 0; i < 8; i++)
    {
        cellOutline.setSize(sf::Vector2f(cellSizes[i], 20.f));
        target.draw(cellOutline);
        tradeText.setString(headers[i]);
        target.draw(tradeText);
        cellOutline.move(cellSizes[i], 0);
        tradeText.move(cellSizes[i], 0);
    }
    tradeText.setCharacterSize(size);
    cellOutline.setFillColor(sf::Color(200, 200, 200));
    cellOutline.move(-sumOfArr, 20);
    tradeText.move(-sumOfArr, 20);

    for (Equipment* item : bodysEquipment)
    {
        for (int i = 0; i < 8; i++)
        {
            cellOutline.setSize(sf::Vector2f(cellSizes[i], 20.f));
            target.draw(cellOutline);
            tradeText.setString(functions[i](*static_cast<const Equipment*>(item)));
            target.draw(tradeText);
            cellOutline.move(cellSizes[i], 0);
            tradeText.move(cellSizes[i], 0);
        }
        cellOutline.move(-sumOfArr, 20);
        tradeText.move(-sumOfArr, 20);
    }
}

void Pawns::drawTrade(sf::RenderTarget& target)
{
    Pawn* pawn = pawnDict[whichPawn];
    Pawn* body = pawnDict[numberOfPawn(pawn->getHexCoords(), true)];
    std::vector<Equipment*> bodysEquipment = body->getEquipment();
    drawTable(target, bodysEquipment);
    
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
