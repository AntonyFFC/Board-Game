#include "Pawns.h"

Pawns::Pawns(Board* board)
    :board(board)
{
    previous = empty;
    wasShift = false;
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
                break;
            }

            if (board->hexDict[present]->isPawn())
            {
                if (previous != empty) //this checks if this the second click of a player
                {
                    std::vector<std::tuple<int, int, int>> inView = getViewOfPawn(previous);
                    auto it = std::find(inView.begin(), inView.end(), present);
                    if (it != inView.end())
                    {
                        attack(previous, present);
                        board->clearHighlight();
                        if (pawnDict[previous]->getRemainingActions() == pawnDict[previous]->getMaxActions())
                        {
                            previous = empty;
                        }
                        else
                        {
                            pawnClicked(previous);
                        }
                    }
                }
                else //this occurs when it is the first click of the player
                {
                    pawnClicked(present);
                    previous = present;
                }
            }
            else if (board->hexDict[present]->isHigh(0))
            {
                pawnMoved(previous, present);
                board->clearHighlight();
                if (pawnDict[present]->getRemainingActions() == pawnDict[present]->getMaxActions())
                {
                    previous = empty;
                }
                else
                {
                    pawnClicked(present);
                    previous = present;
                }
            }
            break;
        }
    }
}

void Pawns::addPawn(Pawn* inPawn, std::tuple<int, int, int> coords)
{
    pawnDict[coords] = inPawn;
    board->hexDict[coords]->setPawn(true, inPawn);
}

void Pawns::handleShiftOn()
{
    if (!wasShift)
    {
        for (auto& pair : pawnDict)
        {
            std::vector<std::tuple<int, int, int>> inView = getViewOfPawn(pair.first);
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

bool Pawns::addItemToPawn(std::tuple<int, int, int> coords, Equipment* item)
{
    return pawnDict[coords]->addEquipment(item);
}

void Pawns::pawnClicked(std::tuple<int, int, int> current)
{
    board->highlighted[0] = getRangeOfPawn(current);
    for (std::tuple<int, int, int> hex : board->highlighted[0])
    {
        board->hexDict[hex]->setHighlight(true, 0);
    }
}

void Pawns::pawnMoved(std::tuple<int, int, int> previous, std::tuple<int, int, int> current)
{
    board->hexDict[previous]->setPawn(false);
    pawnDict[previous]->reduceActions(board->hexDict[current]->getPawnDist());
    if (pawnDict[previous]->getRemainingActions() == 0)
    {
        pawnDict[previous]->setRemainingActions(pawnDict[previous]->getMaxActions());
    }
    board->hexDict[current]->setPawn(true, pawnDict[previous]);
    pawnDict[current] = pawnDict[previous];
    pawnDict.erase(previous);
}

void Pawns::attack(std::tuple<int, int, int> previous, std::tuple<int, int, int> current)
{
    Pawn* attacker = pawnDict[previous];
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
        pawnDict[current]->reduceHP(weapon->getAttackValue());
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

std::vector<std::tuple<int, int, int>> Pawns::getViewOfWeapon(std::tuple<int, int, int> coords, Equipment* weapon)
{
    int range = weapon->getRange();
    return board->getInView(coords, range, 0);
}

std::vector<std::tuple<int, int, int>> Pawns::getViewOfPawn(std::tuple<int, int, int> coords)
{
    Pawn* pawn = pawnDict[coords];
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
        inView = getViewOfWeapon(coords, weapon);
    }
    return inView;
}

std::vector<std::tuple<int, int, int>> Pawns::getRangeOfPawn(std::tuple<int, int, int> coords)
{
    Pawn* pawn = pawnDict[coords];
    std::vector<std::tuple<int, int, int>> inRange;
    inRange = board->getReachable(coords, pawn->getRemainingActions());
    return inRange;
}
