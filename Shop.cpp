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
	equipmentList = EquipmentManager::loadEquipmentFromJson("equipment");
	pawnsList = PawnsManager::loadPawnsFromJson("pawns");
	backgroundSprite = loadBackgroundSprite(&backgroundTexture, "shop");
	backgroundSprite.setPosition(0, 0);
	changeButton = Button(sf::Vector2f(window->getSize().x / 2 - 100,
		window->getSize().y - 70), sf::Vector2f(200, 50), "-}");
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
	std::cout << "Player: " << player << "Card:" << card << std::endl;
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
	window->draw(titleText);
	drawCards();
	drawChangeButton();
	window->display();
}

void Shop::drawCards()
{
	sf::Vector2f pos(500, 20);
	for (auto& cardPtr : shownCards)
	{
		cardPtr->setPosition(pos);
		cardPtr->setScale(0.9);
		window->draw(cardPtr->getSprite());
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
		else if (changeButton.isClicked(mousePosition))
		{
			flipPage();
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
	shownCards.clear();
	if (currentPage)
	{
		for (Pawn* warrior : availableWarriors)
		{
			shownCards.push_back(std::make_shared<WarriorCard>(warrior));
		}
	}
	else
	{
		for (Equipment* item : availableItems)
		{
			shownCards.push_back(std::make_shared<EquipmentCard>(item));
		}
	}
}

void Shop::flipPage()
{
	currentPage = !currentPage;
	initializeCards();
	displayShop();
}
