#include "Shop.h"

Shop::Shop(sf::RenderWindow* window)
	:window(window)
{
	initializeFont();
	currentRound = 0;
	currentPlayerIndex = 0;
	remainingGold = 6;
	equipmentList = EquipmentManager::loadEquipmentFromJson("equipment");
	pawnsList = PawnsManager::loadPawnsFromJson("pawns");
	backgroundSprite = loadBackgroundSprite(&backgroundTexture, "shop");
	backgroundSprite.setPosition(0, 0);
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
	initializeDecks();
	initializeCards();
}

bool Shop::buy(int player, int card)
{
	return false;
}

void Shop::nextTurn()
{
	currentPlayerIndex = (currentPlayerIndex + 1) % 2;
	currentRound++;
}

void Shop::displayShop()
{
	window->clear(sf::Color(71, 31, 16));
	window->draw(backgroundSprite);
	for (auto& cardPtr : shownCards)
	{
		window->draw(cardPtr->getSprite());
	}
	window->display();
}

void Shop::resetShop()
{
}

void Shop::updateShop()
{
}

void Shop::keyPressed(const sf::Event& event)
{
	if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
		sf::Vector2i mousePosition = sf::Mouse::getPosition(*window);
		int cardNum = whichCardClicked(mousePosition);
		if (cardNum != -1)
		{
			buy(currentPlayerIndex, cardNum);
		}
	}
	else if (event.type == sf::Event::Closed)
	{
		window->close();
	}
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

void Shop::initializeDecks()
{
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
}

void Shop::initializeCards()
{
	for (Equipment* item : availableItems)
	{
		shownCards.push_back(std::make_shared<EquipmentCard>(item));
	}
}
