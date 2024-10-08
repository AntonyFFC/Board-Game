#include "Pawns.h"

Pawns::Pawns(Board* board,sf::RenderWindow* window)
    :board(board),target(window)
{
	walls = new Walls(window);
    tradeTable = nullptr;
	currentTable = new Table(target);// add delete at the end chosen!!!
    previousHex = empty;
    wasShift = false;
    isTrading_ = false;
    whosTurn = 0;
    whichPawn = 0;
    previousWarrior[0] = -1;
    previousWarrior[1] = -1;
    numberOfPawns[0] = 0;
    numberOfPawns[1] = 0;
    setupText();
}

Pawns::~Pawns()
{
    delete currentTable;
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
            current = pair.first;
            if (board->hexDict[current]->isClicked(mousePosition)) {
                if ((!board->hexDict[current]->isPawn() || !board->hexDict[current]->pawn->isAlive()) && !board->hexDict[current]->isHigh(0))
                {
                    board->clearHighlight();
                    previousHex = empty;
                }
                else if (board->hexDict[current]->isPawn() && board->hexDict[current]->pawn->isAlive())
                {
                    if (previousHex != empty)
                    {
                        pawnSecond(whichPawn);
                    }
                    else
                    {
                        if (!pawnFirst())
                        {
                            break;
                        }
                    }
                }
                else
                {
                    pawnMoved(whichPawn);
                }
                break;
            }
        }
    }
}

void Pawns::pawnSecond(int pawnNum)
{
    int attackedNum = numberOfPawn(current);
    std::vector<std::tuple<int, int, int>> inView = getViewOfPawn(pawnNum);
    auto it = std::find(inView.begin(), inView.end(), current);
    if (it != inView.end())
    {
        std::vector<Equipment*> weaponsInView = getWeaponsInUse(pawnNum, attackedNum);
        if (weaponsInView.size())
        {
			currentTable->setPosition(sf::Vector2f(target->getSize().x - 375, 35));
			currentTable->setEquipment(weaponsInView);
            currentTable->createTexture();
            setChoosing(true);
        }
        else
        {
            std::cout << "No weapon\n";
        }
    }
}

bool Pawns::pawnFirst()
{
    whichPawn = numberOfPawn(current);
    if (whosTurn == pawnDict[whichPawn]->getSide() 
        && (whichPawn != previousWarrior[whosTurn] || numberOfPawns[whosTurn]==1))
    {
        pawnClicked(whichPawn);
        previousHex = current;
        previousWarrior[whosTurn] = whichPawn;
        return true;
    }
    else {
        board->clearHighlight();
        previousHex = empty;
        return false; //stop the loop
    }
}

void Pawns::handleClickRight(sf::Vector2i mousePosition)
{
    std::tuple<int, int, int> pawnCoords = pawnDict[whichPawn]->getHexCoords();
    if (board->hexDict[pawnCoords]->isClicked(mousePosition) )
    {
        if (board->hexDict[pawnCoords]->hasBody())
        {
            board->clearHighlight();
            pawnDict[whichPawn]->reduceActions(1);
            setTrading(true);
            tradeTable = new TradeTable(pawnDict[whichPawn], pawnDict[numberOfPawn(pawnCoords, true)], target);
            resetTurn();
        }
        else
        {
			pawnDict[whichPawn]->toggleIsEquipmentShown(true);
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
                        previousHex = empty;
                    }
                }
                else if (board->hexDict[coords]->isPawn())
                {
                    pawnDict[numberOfPawn(coords)]->toggleIsEquipmentShown();
                }
                else if (board->hexDict[coords]->hasBody())
                {
                    pawnDict[numberOfPawn(coords, true)]->toggleIsEquipmentShown();
                }
                else if (!placeWall(whichPawn, coords))
                {
                    board->clearHighlight();
                    previousHex = empty;
                }
                return;
            }
        }

        for (Pawn* pawn : pawnDict)
        {
            if (pawn->isClicked(mousePosition))
            {
				pawn->toggleIsEquipmentShown();
                return;
            }
        }
    }
}

bool Pawns::placeWall(int pawnNumber, std::tuple<int, int, int> coords)
{
    if (!walls->buildWall(pawnDict[pawnNumber]->getSide()))
    {
		return false;
    }
    board->clearHighlight();
    pawnDict[pawnNumber]->reduceActions(1);
    board->setWall(coords);
    resetTurn();
	return true;
}

bool Pawns::destroyWall(int pawnNumber, std::tuple<int, int, int> coords)
{
    board->clearHighlight();
	int wallDestroyCost = pawnDict[pawnNumber]->getWallDestroyCost();
    if (pawnDict[pawnNumber]->getRemainingActions() < wallDestroyCost)
    {
        return false;
    }
    else
    {
        board->setGrass(coords);
        pawnDict[pawnNumber]->reduceActions(wallDestroyCost);
        resetTurn();
    }
    return true;
}

void Pawns::addPawn(Pawn* inPawn)
{
    pawnDict.push_back(inPawn);
    board->hexDict[inPawn->getHexCoords()]->setPawn(true, inPawn);
    numberOfPawns[inPawn->getSide()]++;
}

void Pawns::addWalls(int numWalls, int teamNum)
{
	walls->addWalls(numWalls, teamNum);
}

void Pawns::handleShiftOn()
{
    if (!wasShift)
    {
        for (int i = 0; i<pawnDict.size(); i++)
        {
            std::vector<std::tuple<int, int, int>> inView = getViewOfPawn(i);
            for (std::tuple<int, int, int> hex : inView)
            {
                if (board->hexDict[hex]->isHigh(1))
                {
                    board->hexDict[hex]->setHighlight(2, pawnDict[i]->getSide());
                    continue;
                }
                board->hexDict[hex]->setHighlight(1, pawnDict[i]->getSide());
            }
            board->highlighted[1].insert(board->highlighted[1].end(), inView.begin(), inView.end());
        }
    }
    wasShift = true;
}

void Pawns::handleShiftOff()
{
    for (std::tuple<int, int, int> hex : board->highlighted[1])
    {
        board->hexDict[hex]->clearHighlight(1);
        board->hexDict[hex]->clearHighlight(2);
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
    walls->draw();
    if (isTrading())
    {
        tradeTable->draw();
    }
    else if (isChoosing()) {
        currentTable->draw();
    }
}

void Pawns::drawPawns(bool isShift)
{
    for (auto it = pawnDict.rbegin(); it != pawnDict.rend(); ++it) {
        auto& pawn = *it;
        int x = board->hexDict[pawn->getHexCoords()]->getOrigin().x;
        int y = board->hexDict[pawn->getHexCoords()]->getOrigin().y;
        pawn->setPosition(x, y);
        pawn->draw(*target, isShift);
    }
    for (Pawn* pawn : pawnDict) {
        pawn->drawTable(dynamic_cast<sf::RenderWindow*>(target));
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
        board->hexDict[hex]->setHighlight(0, whosTurn);
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
    if (currentTable->tableClicked(mousePosition))
    {
        std::cout << "Table clicked\n";
        item = currentTable->getClickedItem(mousePosition);
        if (item != nullptr)
        {
            int attackerNum = whichPawn;
            int attackedNum = numberOfPawn(current);
            setChoosing(false);
            attack(attackerNum, attackedNum, item);
        }
    }
}

void Pawns::pawnMoved(int pawnNum)
{
    Pawn* pawn = pawnDict[pawnNum];
    pawn->reduceActions(board->hexDict[current]->getPawnDist());
    board->hexDict[previousHex]->setPawn(false);
    board->hexDict[current]->setPawn(true, pawn);
    previousHex = current;
    resetTurn();
}

void Pawns::attack(int pawnNum, int attackedNum, Equipment* weapon)
{
    Pawn* attacker = pawnDict[pawnNum];
    if (weapon->getAttackActions() <= attacker->getRemainingActions())
    {
        Pawn* attacked = pawnDict[attackedNum];
		int attackValue = weapon->getAttackValue();
		if (pawnDict[pawnNum]->hasItem("gauntlets"))
		{
			attackValue++;
		}

        if (weapon->isRanged())
        {
			int missMax = pawnDict[pawnNum]->getMissMax(weapon->getName());
            attacked->rangedAttack(attackValue, missMax);
        }
        else
        {
            attacked->attack(attackValue);
			if (weapon->getName() == "dagger" || weapon->getName() == "long dagger")
			{
				attacker->removeEquipment(weapon);
			}
        }

        if (!attacked->isAlive())
        {
            death(attacked);
        }
        attacker->reduceActions(weapon->getAttackActions());
        resetTurn();
    }
    else
    {
        std::cout << "Not enough actions\n";
    }
}

void Pawns::death(Pawn* attacked)
{
    board->hexDict[attacked->getHexCoords()]->setPawn(false);
    attacked->addSpace(10, 10); //this is so the body does not have a low limit of space
    board->hexDict[attacked->getHexCoords()]->setBody(true, attacked);
    numberOfPawns[attacked->getSide()]--;
}

std::vector<Equipment*> Pawns::getWeaponsInUse(int pawnNum, int attackedNum)
{
    std::vector<Equipment*> items;
    std::tuple<int, int, int> attackedSpot = pawnDict[attackedNum]->getHexCoords();
    Pawn* attacker = pawnDict[pawnNum];
    for (Equipment* item : attacker->getEquipment())
    {
        if (item->getType() == "Weapon" && item->getAttackActions() <= attacker->getRemainingActions()
            || item->getName() == "dagger" || item->getName() == "long dagger")
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

void Pawns::resetTurn()
{
    board->clearHighlight();
    pawnClicked(whichPawn);
}

void Pawns::endTurn()
{
    pawnDict[whichPawn]->setRemainingActions(pawnDict[whichPawn]->getMaxActions());
    board->clearHighlight();
    previousHex = empty;
    flipTurn();
}
