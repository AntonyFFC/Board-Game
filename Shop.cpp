#include "Shop.h"

Shop::Shop(sf::RenderWindow* window)
	:window(window)
{
	initializeFont();
	fontSize = 20;
	currentRound = 1;
	currentPlayerIndex = 0;
	currentPage = 0;
	remainingGold = 6;
	titleText = initializeText("Shop", &globalFont2, fontSize * 1.5, sf::Color::White);
	titleText.setPosition(20, 10);
	blueTurnText = initializeText("Blue", &globalFont2, fontSize * 1.5, sf::Color::Blue);
	blueTurnText.setPosition(20, 50);
	redTurnText = initializeText("Red", &globalFont2, fontSize * 1.5, sf::Color::Red);
	redTurnText.setPosition(20, 50);
	goldText = initializeText("Gold: "+std::to_string(remainingGold), &globalFont2, fontSize * 1.5, sf::Color(222, 164, 38));
	goldText.setPosition(window->getSize().x-100, 10);
	roundText = initializeText("Round: " + std::to_string(currentRound), &globalFont2, fontSize * 1.5, sf::Color::White);
	roundText.setPosition(window->getSize().x - 250, 10);
	equipmentList = EquipmentManager::loadEquipmentFromJson("equipment");
	pawnsList = PawnsManager::loadPawnsFromJson("pawns");
	backgroundSprite = loadBackgroundSprite(&backgroundTexture, "shop");
	backgroundSprite.setPosition(0, 0);
	changeButton = Button(sf::Vector2f(window->getSize().x / 2 - 100,
		window->getSize().y - 70), sf::Vector2f(200, 50), "-}");
	nextPlayer = Button(sf::Vector2f(window->getSize().x / 2 + 150,
		window->getSize().y - 70), sf::Vector2f(200, 50), "next player");
	wallIcon.setPosition(20, window->getSize().y / 2);
	interface1 = new Gui(window);
}

Shop::~Shop()
{
	delete interface1;
	for (WarriorCard* warriorCard : warriorsCards)
	{
		delete warriorCard;
	}
	warriorsCards.clear();
	for (EquipmentCard* itemCard : itemsCards)
	{
		delete itemCard;
	}
	itemsCards.clear();
}

void Shop::start()
{
	updateDecks();
	displayShop();
	while (window->isOpen())
	{
		sf::Event event;
		while (window->pollEvent(event)) {
			keyPressed(event); //this also calls display
		}
	}
}

bool Shop::buy(int cardNum)
{
	int price = getPrice(cardNum);
	if (remainingGold < price )
	{
		return false;
	}

	addLastItemToStorage();

	addCard(cardNum);
	removeShopCard(cardNum);
	reduceMoney(price);
	updateGoldText();
	return true;
}

bool Shop::buyWall()
{
	if (remainingGold < 1)
	{
		return false;
	}
	addLastItemToStorage();
	wallIcon.setIsBeingClicked(false);
	shopStorage[currentPlayerIndex].addWall();
	reduceMoney(1);
	updateGoldText();
	return true;
}

int Shop::getPrice(int cardNum)
{
	if (currentPage)
	{
		return itemsCards[cardNum]->getPrice();
	}
	else
	{
		return warriorsCards[cardNum]->getPrice();
	}
}

void Shop::addCard(int cardNum)
{
	if (currentPage)
	{
		EquipmentCard* itemCard = itemsCards[cardNum];
		lastItem = itemCard;
	}
	else
	{
		WarriorCard* warriorCard = warriorsCards[cardNum];
		warriorCard->getWarrior()->setSide(currentPlayerIndex);
		shopPawns[currentPlayerIndex].addPawn(warriorCard->getWarrior());
	}
}

void Shop::removeShopCard(int cardNum)
{
	if (currentPage)
	{
		EquipmentCard* itemCard = itemsCards[cardNum];
		//here I remove from all items so that they are not drawn again
		equipmentList.erase(std::remove(equipmentList.begin(), equipmentList.end(), itemCard->getItem()), equipmentList.end());
		//here I remove from the cards, which are in the shop
		itemsCards.erase(itemsCards.begin() + cardNum);
	}
	else
	{
		WarriorCard* warriorCard = warriorsCards[cardNum];
		//here I remove from all warriors so that they are not drawn again
		pawnsList.erase(std::remove(pawnsList.begin(), pawnsList.end(), warriorCard->getWarrior()), pawnsList.end());
		//here I remove from the cards, which are in the shop
		warriorsCards.erase(warriorsCards.begin() + cardNum);
	}
}

void Shop::reduceMoney(int price)
{
	remainingGold -= price;
}

void Shop::addLastItemToStorage()
{
	if (lastItem != nullptr)
	{
		shopStorage[currentPlayerIndex].addCard(lastItem);
		lastItem = nullptr;
	}
}

void Shop::nextTurn()
{
	addLastItemToStorage();
	remainingGold = 6;
	currentPlayerIndex = (currentPlayerIndex + 1) % 2;
	if (!currentPlayerIndex)
	{
		currentRound++;
		if (currentRound == 2)//shold be 7
		{
			startGame();
			return;
		}
	}
	std::string text = "Round: " + std::to_string(currentRound);
	roundText.setString(text);
	updateDecks();
	updateGoldText();
}

void Shop::displayShop()
{
	window->clear(sf::Color(71, 31, 16));
	window->draw(backgroundSprite);
	window->draw(titleText);
	drawTurn();
	window->draw(goldText);
	window->draw(roundText);
	drawCards();
	shopStorage[currentPlayerIndex].draw(window);
	shopPawns[currentPlayerIndex].draw(window);
	drawChangeButton();
	drawNextPlayerButton();
	wallIcon.draw(*window);
	window->display();
}

void Shop::drawCards()
{
	sf::Vector2f pos(window->getSize().x / 4, 20);
	if (currentPage)
	{
		for (auto& itemCard : itemsCards)
		{
			itemCard->setScale(0.9);
			itemCard->setPosition(sf::Vector2f(pos.x - itemCard->getSprite().getGlobalBounds().width / 2, pos.y));
			window->draw(itemCard->getFullSprite());
			pos += sf::Vector2f(0, itemCard->getSprite().getTextureRect().height + 10);
		}
	}
	else
	{
		for (auto& warriorCard : warriorsCards)
		{
			warriorCard->setScale(0.9);
			warriorCard->setPosition(sf::Vector2f(pos.x - warriorCard->getSprite().getGlobalBounds().width / 2, pos.y));
			window->draw(warriorCard->getFullSprite());
			pos += sf::Vector2f(0, warriorCard->getSprite().getTextureRect().height + 10);
		}
	}
}

void Shop::drawChangeButton()
{
	if (currentPage)
	{
		changeButton.setText("{-");
	}
	else
	{
		changeButton.setText("-}");
	}
	changeButton.draw(*window);
}

void Shop::drawNextPlayerButton()
{
	if (remainingGold == 0)
	{
		nextPlayer.draw(*window);
	}
}

void Shop::drawTurn()
{
	if (currentPlayerIndex)
	{
		window->draw(blueTurnText);
		return;
	}
	window->draw(redTurnText);
}

void Shop::resetShop()
{
}

void Shop::keyPressed(const sf::Event& event)
{
	sf::Vector2i mousePosition = sf::Mouse::getPosition(*window);
	if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
		whatClicked(mousePosition);
		displayShop();
	} else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
		unClickAll();
		whatOffClicked(mousePosition);
		displayShop();
	}
	else if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::Right || event.key.code == sf::Keyboard::Left)
		{
			flipPage();
		}
		displayShop();
	}
	else if (event.type == sf::Event::Closed)
	{
		window->close();
	}
}

void Shop::whatClicked(sf::Vector2i mousePosition)
{
	int cardNum = whichCardClicked(mousePosition);
	int pawnNum = shopPawns[currentPlayerIndex].whichPawnClicked(mousePosition);
	int storageCardNum = shopStorage[currentPlayerIndex].whichItemClicked(mousePosition);
	if (cardNum != -1)
	{
		clickCard(cardNum);
	}
	else if (changeButton.isClicked(mousePosition))
	{
		flipPage();
	}
	else if (nextPlayer.isClicked(mousePosition) && !remainingGold)
	{
		nextTurn();
	}
	else if (wallIcon.isClicked(mousePosition))
	{
		wallIcon.setIsBeingClicked(true);
	}
	else if (pawnNum != -1 && lastItem != nullptr)
	{
		if (!shopPawns[currentPlayerIndex].addEquipmentToPawn(pawnNum, lastItem->getItem()))
		{
			shopStorage[currentPlayerIndex].addCard(lastItem);
		}
		lastItem = nullptr;
	}
	else if (storageCardNum != -1)
	{
		lastItem = shopStorage[currentPlayerIndex].takeItem(storageCardNum);
	}
	else if (shopStorage[currentPlayerIndex].isClicked(mousePosition) && lastItem != nullptr)
	{
		shopStorage[currentPlayerIndex].addCard(lastItem);
		lastItem = nullptr;
	}
}

void Shop::whatOffClicked(sf::Vector2i mousePosition)
{
	int cardNum = whichCardClicked(mousePosition);
	if (cardNum != -1)
	{
		buy(cardNum);
	}
	else if (wallIcon.isClicked(mousePosition))
	{
		buyWall();
	}
}

int Shop::whichCardClicked(sf::Vector2i mousePosition)
{
	int i = 0;
	if (currentPage)
	{
		for (const auto& cardPtr : itemsCards)
		{
			if (cardPtr->isClicked(mousePosition))
			{
				return i;
			}
			i++;
		}
	}
	else
	{
		for (const auto& cardPtr : warriorsCards)
		{
			if (cardPtr->isClicked(mousePosition))
			{
				return i;
			}
			i++;
		}
	}
	return -1;
}

void Shop::clickCard(int cardNum)
{
	if (currentPage)
	{
		itemsCards[cardNum]->click(true);
	}
	else
	{
		warriorsCards[cardNum]->click(true);
	}
}

void Shop::updateDecks()
{
	std::vector<Equipment*> availableItems;
	std::vector<Pawn*> availableWarriors;
	// Randomly select 3 warriors from pawnsList
	if (pawnsList.size() > 3) {
		std::random_device rd;
		std::mt19937 gen(rd());
		std::shuffle(pawnsList.begin(), pawnsList.end(), gen);
		availableWarriors.assign(pawnsList.begin(), pawnsList.begin() + 3);
	}
	else {
		// Handle the case where there are fewer than 3 pawns
		availableWarriors = pawnsList;
	}

	// Randomly select 5 equipment items from equipmentList
	if (equipmentList.size() > 5) {
		std::random_device rd;
		std::mt19937 gen(rd());
		std::shuffle(equipmentList.begin(), equipmentList.end(), gen);
		availableItems.assign(equipmentList.begin(), equipmentList.begin() + 5);
	}
	else {
		// Handle the case where there are fewer than 5 equipment items
		availableItems = equipmentList;
	}
	initializeCards(availableItems, availableWarriors);
}

void Shop::initializeCards(std::vector<Equipment*> availableItems, std::vector<Pawn*> availableWarriors)
{
	warriorsCards.clear();
	itemsCards.clear();
	for (Pawn* warrior : availableWarriors)
	{
		WarriorCard* warriorCard = new WarriorCard(warrior);
		warriorsCards.push_back(warriorCard);
	}
	for (Equipment* item : availableItems)
	{
		EquipmentCard* equipmentCard = new EquipmentCard(item);
		itemsCards.push_back(equipmentCard);
	}
}

void Shop::flipPage()
{
	currentPage = !currentPage;
}

void Shop::updateGoldText()
{
	std::string text = "Gold: " + std::to_string(remainingGold);
	goldText.setString(text);
}

void Shop::unClickAll()
{
	wallIcon.setIsBeingClicked(false);
	if (currentPage)
	{
		for (const auto& cardPtr : itemsCards)
		{
			cardPtr->click(false);
		}
	}
	else
	{
		for (const auto& cardPtr : warriorsCards)
		{
			cardPtr->click(false);
		}
	}
}

void Shop::startGame()
{
	interface1->addPawns(shopPawns[0].getPawns(), 0);
	interface1->addPawns(shopPawns[1].getPawns(), 1);
	interface1->start();
}
