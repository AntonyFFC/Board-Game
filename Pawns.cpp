#include "Pawns.h"

Pawns::Pawns(Board* board)
    :board(board)
{
    previous = empty;
    wasShift = false;
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

int Pawns::numberOfPawn(std::tuple<int, int, int> coords)
{
    int keyValue = -1;
    Pawn* targetObject = board->hexDict[coords]->pawn;

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
            if (!board->hexDict[present]->isPawn() && !board->hexDict[present]->isHigh(0))
            {
                board->clearHighlight();
                previous = empty;
            }
            else if (board->hexDict[present]->isPawn())
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

bool Pawns::addItemToPawn(int number, Equipment* item)
{
    return pawnDict[number]->addEquipment(item);
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
    for (Equipment* item : attacker->getEquipment())
    {
        if (item->getType() == "Weapon")
        {
            weapon = item;
        }
    }
    if (weapon != nullptr)
    {
        pawnDict[attackedNum]->reduceHP(weapon->getAttackValue());
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
    int range = weapon->getRange();
    return board->getInView(pawn->getHexCoords(), range, 0);
}

std::vector<std::tuple<int, int, int>> Pawns::getViewOfPawn(int pawnNum)
{
    Pawn* pawn = pawnDict[pawnNum];
    Equipment* weapon = nullptr;
    int range = 0;
    std::vector<std::tuple<int, int, int>> inView;
    for (Equipment* item : pawn->getEquipment())
    {
        if (item->getType() == "Weapon" && item->getRange() > range)
        {
            weapon = item;
            range = item->getRange();
        }
    }
    if (weapon != nullptr)
    {
        inView = getViewOfWeapon(pawnNum, weapon);
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
