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
    delete walls;
	for (EquipmentPile* pile : pileDict) {
		delete pile;
	}
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
        return;
    }

    if (isChoosing())
    {
        choosing(mousePosition);
        return;
    }

    if (isDropping())
    {
        if (dropping(mousePosition))
        {
            return;
        }
    }

    for (auto& pair : board->hexDict) {
        current = pair.first;
        if (!board->hexDict[current]->isClicked(mousePosition))
        {
            continue;
        }

        if (shouldClearHighlight(current))
        {
            board->clearHighlight();
            previousHex = empty;
        }
        else if (isItAlivePawn(current))
        {
            handlePawnClickInteraction();
        }
        else
        {
            pawnMoved(whichPawn);
        }
        break;
    }
}

bool Pawns::shouldClearHighlight(const std::tuple<int, int, int>& hex) const
{
    return (!board->hexDict[hex]->isPawn() || !board->hexDict[hex]->pawn->isAlive()) &&
        !board->hexDict[hex]->isHigh(0);
}

bool Pawns::isItAlivePawn(const std::tuple<int, int, int>& hex) const
{
    return board->hexDict[hex]->isPawn() && board->hexDict[hex]->pawn->isAlive();
}

void Pawns::handlePawnClickInteraction()
{
    if (previousHex != empty)
    {
        pawnSecond(whichPawn);
    }
    else
    {
        pawnFirst();
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
        pawnFirstClick(whichPawn);
        previousHex = current;
        previousWarrior[whosTurn] = whichPawn;
        return true;
    }
    else {
        board->clearHighlight();
        previousHex = empty;
        return false; //stop the loop - no it stops anyway becouse there is a break after clicking that hex
    }
}

void Pawns::handleClickRelease(sf::Vector2i mousePosition)
{
    if (isDropping())
    {
        if (drop(mousePosition))
        {
            return;
        }
    }
}

void Pawns::handleClickRight(sf::Vector2i mousePosition)
{
    if (previousHex == empty)
    {
        for (auto& pair : board->hexDict) {
            if (!board->hexDict[pair.first]->isClicked(mousePosition))
            {
                continue;
            }
            pawnFirstRight(pair.first);
        }
    }
    std::tuple<int, int, int> pawnCoords = pawnDict[whichPawn]->getHexCoords();
    if (board->hexDict[pawnCoords]->isClicked(mousePosition) )
    {
        if (board->hexDict[pawnCoords]->hasBody())
        {
            handleRightClickOnTrade(pawnCoords, true);
            return;
        }
        else if (board->hexDict[pawnCoords]->hasEquipmentPile())
        {
            handleRightClickOnTrade(pawnCoords, false);
            return;
        }

		pawnDict[whichPawn]->toggleIsEquipmentShown(true);
        return;
    }

    if (handleNeighborRightClick(mousePosition, pawnCoords))
    {
        return;
    }

    handlePawnEquipmentRightClick(mousePosition);
}

bool Pawns::pawnFirstRight(std::tuple<int, int, int> pawnCoords)
{
    whichPawn = numberOfPawn(pawnCoords);
    if (whosTurn == pawnDict[whichPawn]->getSide()
        && (whichPawn != previousWarrior[whosTurn] || numberOfPawns[whosTurn] == 1))
    {
        previousHex = pawnCoords;
        previousWarrior[whosTurn] = whichPawn;
        return true;
    }
    else {
        board->clearHighlight();
        previousHex = empty;
        return false;
    }
}

void Pawns::handleRightClickOnTrade(const std::tuple<int, int, int>& coords, bool isBody)
{
    if (pawnDict[whichPawn]->getRemainingActions() <= 0)
    {
        return;
    }

    board->clearHighlight();
    pawnDict[whichPawn]->reduceActions(1);
    setTrading(true);

    if (isBody)
    {
        tradeTable = new TradeTable(pawnDict[whichPawn], pawnDict[numberOfPawn(coords, true)]->getEquipment(), target);
    }
    else
    {
        EquipmentPile* targetPile = pileDict[numberOfPile(coords)];
        tradeTable = new TradeTable(pawnDict[whichPawn], targetPile->getEquipment(), target);
    }
    resetTurn();
}

int Pawns::numberOfPile(const std::tuple<int, int, int>& pileCoords) const
{
    for (size_t i = 0; i < pileDict.size(); ++i)
    {
        if (pileDict[i]->getHexCoords() == pileCoords)
        {
            return static_cast<int>(i);
        }
    }
    throw std::runtime_error("No EquipmentPile found at the given coordinates");
}

bool Pawns::handleNeighborRightClick(sf::Vector2i mousePosition, const std::tuple<int, int, int>& pawnCoords)
{
    for (const auto& coords : board->getNeighbours(pawnCoords))
    {
        if (!board->hexDict[coords]->isClicked(mousePosition))
        {
            continue;
        }

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
        return true;
    }
    return false;
}

void Pawns::handlePawnEquipmentRightClick(sf::Vector2i mousePosition)
{
    for (Pawn* pawn : pawnDict)
    {
        if (pawn->isClicked(mousePosition))
        {
            pawn->toggleIsEquipmentShown();
            return;
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

bool Pawns::isDropping() const {
	return pawnDict[whichPawn]->getIsEquipmentShown();
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
    for (EquipmentPile* pile : pileDict) {
        pile->setPosition(board->hexDict[pile->getHexCoords()]->getOrigin().x,
            board->hexDict[pile->getHexCoords()]->getOrigin().y);
        pile->draw(*target);
    }
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

void Pawns::pawnFirstClick(int pawnNum)
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
        if (tradeTable->isVectorEmpty() &&
            board->hexDict[pawnDict[whichPawn]->getHexCoords()]->hasEquipmentPile())
        {
			for (EquipmentPile* pile : pileDict) {
				if (pile->getHexCoords() == pawnDict[whichPawn]->getHexCoords())
				{
					board->hexDict[pawnDict[whichPawn]->getHexCoords()]->setEquipmentPile(false);
					pileDict.erase(std::remove(pileDict.begin(), pileDict.end(), pile), pileDict.end());
					delete pile;
					break;
				}
			}
        }
        delete(tradeTable);
        tradeTable = nullptr;
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

bool Pawns::dropping(sf::Vector2i mousePosition)
{
    if (pawnDict[whichPawn]->clickDropButton(mousePosition))
    {
		return true;
    }

	if (pawnDict[whichPawn]->isEquipmentTableClicked(mousePosition))
	{
		std::cout << "Table clicked\n";
		pawnDict[whichPawn]->toggleHighlightEquipmentTable(mousePosition);
        return true;
	}

    return !pawnDict[whichPawn]->getHighlightedEquipment().empty();
}

bool Pawns::drop(sf::Vector2i mousePosition)
{
    if (canDropItem(whichPawn, mousePosition))
    {
        EquipmentPile* newPile = new EquipmentPile(pawnDict[whichPawn]->getHexCoords());
        pileDict.push_back(newPile);
        pawnDict[whichPawn]->reduceActions(1);
        pawnDict[whichPawn]->dropItems(newPile);
        pawnDict[whichPawn]->setIsEquipmentShown(false);
        board->hexDict[pawnDict[whichPawn]->getHexCoords()]->setEquipmentPile(true);
        resetTurn();
        return true;
    }
    return false;
}

bool Pawns::canDropItem(int pawnNum, sf::Vector2i mousePosition) const
{
    return pawnDict[pawnNum]->unClickDropButton(mousePosition) &&
        pawnDict[pawnNum]->getRemainingActions() && pawnDict[pawnNum]->areAnyHighlighted();
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
    if (!hasEnoughActions(attacker, weapon))
    {
        std::cout << "Not enough actions\n";
        return;
    }

    Pawn* attacked = pawnDict[attackedNum];
    int attackValue = calculateAttackValue(attacker, weapon);

    if (weapon->isRanged())
    {
        handleRangedAttack(attacker, attacked, weapon, attackValue);
    }
    else
    {
        handleMeleeAttack(attacker, attacked, weapon, attackValue);
    }

    if (!attacked->isAlive())
    {
        death(attacked);
    }

    attacker->reduceActions(weapon->getAttackActions());
    resetTurn();
}

bool Pawns::hasEnoughActions(Pawn* attacker, Equipment* weapon) const
{
    return weapon->getAttackActions() <= attacker->getRemainingActions();
}

int Pawns::calculateAttackValue(Pawn* attacker, Equipment* weapon) const
{
    int attackValue = weapon->getAttackValue();
    if (attacker->hasItem("gauntlets"))
    {
        attackValue++;
    }
    return attackValue;
}

void Pawns::handleRangedAttack(Pawn* attacker, Pawn* attacked, Equipment* weapon, int attackValue)
{
    int missMax = attacker->getMissMax(weapon->getName());
    attacked->rangedAttack(attackValue, missMax);
}

void Pawns::handleMeleeAttack(Pawn* attacker, Pawn* attacked, Equipment* weapon, int attackValue)
{
    attacked->attack(attackValue);
    if (weapon->getName() == "dagger" || weapon->getName() == "long dagger")
    {
        attacker->removeEquipment(weapon);
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
        if ((item->getType() == "Weapon" || item->doesNameContain("dagger")) 
            && item->getAttackActions() <= attacker->getRemainingActions())
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
    return board->getInView(pawn, range.maxRange, range.minRange);
}

std::vector<std::tuple<int, int, int>> Pawns::getViewOfPawn(int pawnNum)
{
    Pawn* pawn = pawnDict[pawnNum];
    Equipment* weapon = nullptr;
    std::vector<std::tuple<int, int, int>> inView;
    std::vector<std::tuple<int, int, int>> inViewWeapon;
    for (Equipment* item : pawn->getEquipment())
    {
        if (item->getType() == "Weapon" || item->doesNameContain("dagger"))
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
    return board->getReachable(pawn);
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
    pawnFirstClick(whichPawn);
}

void Pawns::closeTables()
{
	for (Pawn* pawn : pawnDict)
	{
		pawn->setIsEquipmentShown(false);
	}
	if (tradeTable != nullptr)
	{
		delete tradeTable;
		tradeTable = nullptr;
	}
	setTrading(false);
	setChoosing(false);
}

void Pawns::endTurn()
{
    pawnDict[whichPawn]->setRemainingActions(pawnDict[whichPawn]->getMaxActions());
    closeTables();
    board->clearHighlight();
    previousHex = empty;
    flipTurn();
}
