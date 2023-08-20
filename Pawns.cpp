#include "Pawns.h"

Pawns::Pawns(Board* board,sf::RenderWindow* window)
    :board(board),target(window)
{
    tradeTable = nullptr;
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
    if (isTrading())
    {
        trading(mousePosition);
    }
    else if (isChoosing())
    {
        choosing(mousePosition);
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
                    pawnMoved(whichPawn, present);
                }
                break;
            }
        }
    }
}

void Pawns::pawnSecond(int pawnNum, std::tuple<int, int, int> current)
{
    int attackedNum = numberOfPawn(current);
    std::vector<std::tuple<int, int, int>> inView = getViewOfPawn(pawnNum);
    auto it = std::find(inView.begin(), inView.end(), current);
    if (it != inView.end())
    {
        std::vector<Equipment*> weaponsInView = getWeaponsInUse(pawnNum, attackedNum);
        if (weaponsInView.size())
        {
            weaponsTable = new WeaponsTable(pawnDict[pawnNum], pawnDict[attackedNum], weaponsInView, target); // add delete when chosen!!!
            setChoosing(true);
        }
        else
        {
            std::cout << "No weapon\n";
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

void Pawns::handleClickRight(sf::Vector2i mousePosition)
{
    std::tuple<int, int, int> pawnCoords = pawnDict[whichPawn]->getHexCoords();
    if (board->hexDict[pawnCoords]->isClicked(mousePosition) && board->hexDict[pawnCoords]->hasBody())
    {
        board->clearHighlight();
        pawnDict[whichPawn]->reduceActions(1);
        setTrading(true);
        tradeTable = new TradeTable(pawnDict[whichPawn], pawnDict[numberOfPawn(pawnCoords, true)], target);
        resetTurn(whichPawn);
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

void Pawns::placeWall(int pawnNumber, std::tuple<int, int, int> coords)
{
    board->clearHighlight();
    pawnDict[pawnNumber]->reduceActions(1);
    board->setWall(coords);
    resetTurn(pawnNumber);
}

bool Pawns::destroyWall(int pawnNumber, std::tuple<int, int, int> coords)
{
    board->clearHighlight();
    if (pawnDict[pawnNumber]->getRemainingActions() < 4)
    {
        return false;
    }
    else
    {
        board->setGrass(coords);
        pawnDict[pawnNumber]->reduceActions(4);
        resetTurn(pawnNumber);
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

bool Pawns::isChoosing() const {
    return isChoosing_;
}

bool Pawns::addItemToPawn(int number, Equipment* item)
{
    return pawnDict[number]->addEquipment(item);
}

void Pawns::setTrading(bool boolean) {
    isTrading_ = boolean;
}

void Pawns::setChoosing(bool boolean) {
    isChoosing_ = boolean;
}

void Pawns::draw(bool isShift)
{
    drawTurn();
    drawPawns(isShift);
    if (isTrading())
    {
        tradeTable->draw();
    }
    else if (isChoosing()) {
        weaponsTable->draw();
    }
}

void Pawns::drawPawns(bool isShift)
{
    for (auto& pawn : pawnDict) {
        int x = board->hexDict[pawn->getHexCoords()]->getOrigin().x;
        int y = board->hexDict[pawn->getHexCoords()]->getOrigin().y;
        pawn->setPosition(x, y);
        pawn->draw(*target, isShift);
    }
}

void Pawns::drawTurn()
{
    target->draw(turnText);
}

void Pawns::pawnClicked(int pawnNum)
{
    board->highlighted[0] = getRangeOfPawn(pawnNum);
    for (std::tuple<int, int, int> hex : board->highlighted[0])
    {
        board->hexDict[hex]->setHighlight(true, 0);
    }
}

void Pawns::trading(sf::Vector2i mousePosition)
{
    if (tradeTable->tableClicked(mousePosition))
    {
        std::cout << "Table clicked\n";
        tradeTable->trade(mousePosition);
    }
    else if (tradeTable->doneClicked(mousePosition))
    {
        setTrading(false);
        delete(tradeTable);
    }
}

void Pawns::choosing(sf::Vector2i mousePosition)
{
    Equipment* item = nullptr;
    if (weaponsTable->tableClicked(mousePosition))
    {
        std::cout << "Table clicked\n";
        item = weaponsTable->getWeapon(mousePosition);
        if (item != nullptr)
        {
            int attackerNum = numberOfPawn(weaponsTable->getAttacker()->getHexCoords());
            int attackedNum = numberOfPawn(weaponsTable->getAttacked()->getHexCoords());
            setChoosing(false);
            attack(attackerNum, attackedNum, item);
            delete weaponsTable;
        }
    }
}

void Pawns::pawnMoved(int pawnNum, std::tuple<int, int, int> where)
{
    Pawn* pawn = pawnDict[pawnNum];
    pawn->reduceActions(board->hexDict[where]->getPawnDist());
    board->hexDict[previous]->setPawn(false);
    board->hexDict[where]->setPawn(true, pawn);
    previous = where;
    resetTurn(pawnNum);
}

void Pawns::attack(int pawnNum, int attackedNum, Equipment* weapon)
{
    Pawn* attacker = pawnDict[pawnNum];
    if (weapon->getAttackActions() <= attacker->getRemainingActions())
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
            board->hexDict[attacked->getHexCoords()]->setPawn(false);
            attacked->addSpace(8, 8); //this is so the body does not have a low limit of space
            board->hexDict[attacked->getHexCoords()]->setBody(true, attacked);
        }
        attacker->reduceActions(weapon->getAttackActions());
        resetTurn(pawnNum);
    }
    else
    {
        std::cout << "Not enough actions\n";
    }
}

std::vector<Equipment*> Pawns::getWeaponsInUse(int pawnNum, int attackedNum)
{
    std::vector<Equipment*> items;
    std::tuple<int, int, int> attackedSpot = pawnDict[attackedNum]->getHexCoords();
    Pawn* attacker = pawnDict[pawnNum];
    for (Equipment* item : attacker->getEquipment())
    {
        if (item->getType() == "Weapon" && item->getAttackActions() <= attacker->getRemainingActions())
        {
            std::vector<std::tuple<int, int, int>> inView = getViewOfWeapon(pawnNum, item);
            auto it = std::find(inView.begin(), inView.end(), attackedSpot);
            if (it != inView.end())
            {
                items.push_back(item);
            }
        }
    }
    return items;
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

void Pawns::resetTurn(int pawnNum)
{
    board->clearHighlight();
    if (pawnDict[pawnNum]->getRemainingActions() == 0)
    {
        pawnDict[pawnNum]->setRemainingActions(pawnDict[pawnNum]->getMaxActions());
        previous = empty;
        flipTurn();
    }
    else
    {
        pawnClicked(pawnNum);
    }
}