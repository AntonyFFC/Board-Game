#include "Shop.h"

Shop::Shop(sf::RenderWindow* window)
	:window(window)
{
	initializeFont();
	fontSize = 20;
	currentRound = 0;
	currentPlayerIndex = 0;
	currentPage = 0;
	remainingGold = 6;
	titleText = initializeText("Shop", &globalFont2, fontSize * 1.5, sf::Color::White);
	titleText.setPosition(20, 10);
	blueTurnText = initializeText("Turn: Blue", &globalFont2, fontSize * 1.5, sf::Color::Blue);
	blueTurnText.setPosition(10, 50);
	redTurnText = initializeText("Turn: Red", &globalFont2, fontSize * 1.5, sf::Color::Red);
	redTurnText.setPosition(10, 50);
	goldText = initializeText("Gold: "+std::to_string(remainingGold), &globalFont2, fontSize * 1.5, sf::Color::Yellow);
	goldText.setPosition(window->getSize().x-100, 10);
	equipmentList = EquipmentManager::loadEquipmentFromJson("equipment");
	pawnsList = PawnsManager::loadPawnsFromJson("pawns");
	backgroundSprite = loadBackgroundSprite(&backgroundTexture, "shop");
	backgroundSprite.setPosition(0, 0);
	changeButton = Button(sf::Vector2f(window->getSize().x / 2 - 100,
		window->getSize().y - 70), sf::Vector2f(200, 50), "-}");
	wallIcon.setPosition(10, window->getSize().y / 2);
}

void Shop::start()
{
	initializeShop();
	displayShop();
	while (window->isOpen())
	{
		sf::Event event;
		while (window->pollEvent(event)) {
			keyPressed(event); //this also calls display
		}
	}
}

void Shop::initializeShop()
{
	updateDecks();
	assignCards();
}

bool Shop::buy(int cardNum)
{
	std::cout << "Player: " << currentPlayerIndex << " Card:" << cardNum << std::endl;
	int price = shownCards[cardNum]->getPrice();
	if (remainingGold < price)
	{
		return false;
	}

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
	numberOfWalls[currentPlayerIndex]++;
	reduceMoney(1);
	updateGoldText();
	return true;
}

void Shop::addCard(int cardNum)
{
	if (currentPage)
	{
		std::shared_ptr<Card> cardPtr = shownCards[cardNum];
		EquipmentCard* itemCard = dynamic_cast<EquipmentCard*>(cardPtr.get());
		playerItems[currentPlayerIndex].push_back(itemCard->getItem());
	}
	else
	{
		std::shared_ptr<Card> cardPtr = shownCards[cardNum];
		WarriorCard* warriorCard = dynamic_cast<WarriorCard*>(cardPtr.get());
		playerWarriors[currentPlayerIndex].push_back(warriorCard->getWarrior());
	}
}

void Shop::removeShopCard(int cardNum)
{
	if (currentPage)
	{
		std::shared_ptr<Card> cardPtr = shownCards[cardNum];
		EquipmentCard* itemCard = dynamic_cast<EquipmentCard*>(cardPtr.get());
		//here I remove from all items so that they are not drawn again
		equipmentList.erase(std::remove(equipmentList.begin(), equipmentList.end(), itemCard->getItem()), equipmentList.end());
		//here I remove from the cards, which are in the shop
		itemsCards.erase(itemsCards.begin() + cardNum);
		shownCards = itemsCards;
	}
	else
	{
		std::shared_ptr<Card> cardPtr = shownCards[cardNum];
		WarriorCard* warriorCard = dynamic_cast<WarriorCard*>(cardPtr.get());
		//here I remove from all warriors so that they are not drawn again
		pawnsList.erase(std::remove(pawnsList.begin(), pawnsList.end(), warriorCard->getWarrior()), pawnsList.end());
		//here I remove from the cards, which are in the shop
		warriorsCards.erase(warriorsCards.begin() + cardNum);
		shownCards = warriorsCards;
	}
}

void Shop::reduceMoney(int price)
{
	remainingGold -= price;
	if (remainingGold == 0)
	{
		remainingGold = 6;
		nextTurn();
	}

}

void Shop::nextTurn()
{
	currentPlayerIndex = (currentPlayerIndex + 1) % 2;
	if (!currentPlayerIndex)
	{
		currentRound++;
	}
	updateDecks();
	assignCards();
}

void Shop::displayShop()
{
	window->clear(sf::Color(71, 31, 16));
	window->draw(backgroundSprite);
	window->draw(titleText);
	drawTurn();
	window->draw(goldText);
	drawCards();
	drawChangeButton();
	wallIcon.draw(*window);
	window->display();
}

void Shop::drawCards()
{
	sf::Vector2f pos(window->getSize().x / 4, 20);
	for (auto& cardPtr : shownCards)
	{
		cardPtr->setScale(0.9);
		cardPtr->setPosition(sf::Vector2f(pos.x- cardPtr->getSprite().getGlobalBounds().width / 2, pos.y));
		window->draw(cardPtr->getFullSprite());
		pos += sf::Vector2f(0, cardPtr->getSprite().getTextureRect().height + 10);
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
	if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
		sf::Vector2i mousePosition = sf::Mouse::getPosition(*window);
		int cardNum = whichCardClicked(mousePosition);
		if (cardNum != -1)
		{
			shownCards[cardNum]->click(true);
		}
		else if (changeButton.isClicked(mousePosition))
		{
			flipPage();
		}
		else if (wallIcon.isClicked(mousePosition))
		{
			wallIcon.setIsBeingClicked(true);
		}
	} else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
		unClickAll();
		sf::Vector2i mousePosition = sf::Mouse::getPosition(*window);
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
	else if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::Right || event.key.code == sf::Keyboard::Left)
		{
			flipPage();
		}
	}
	else if (event.type == sf::Event::Closed)
	{
		window->close();
	}
	displayShop();
}

int Shop::whichCardClicked(sf::Vector2i mousePosition)
{
	int i = 0;
	for (const auto& cardPtr : shownCards)
	{
		if (cardPtr->isClicked(mousePosition))
		{
			return i;
		}
		i++;
	}
	return -1;
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
		warriorsCards.push_back(std::make_shared<WarriorCard>(warrior));
	}
	for (Equipment* item : availableItems)
	{
		itemsCards.push_back(std::make_shared<EquipmentCard>(item));
	}
}

void Shop::flipPage()
{
	currentPage = !currentPage;
	assignCards();
}

void Shop::assignCards()
{
	if (currentPage)
	{
		shownCards = itemsCards;
	}
	else
	{
		shownCards = warriorsCards;
	}
}

void Shop::updateGoldText()
{
	std::string text = "Gold: " + std::to_string(remainingGold);
	goldText.setString(text);
}

void Shop::unClickAll()
{
	wallIcon.setIsBeingClicked(false);
	for (const auto& cardPtr : shownCards)
	{
		cardPtr->click(false);
	}
}
