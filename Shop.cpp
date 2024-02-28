#include "Shop.h"

Shop::Shop(sf::RenderWindow* window)
	:window(window)
{
	initializeFont();
	fontSize = 20;
	currentRound = 1;
	currentPlayerIndex = 0;
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
	backgroundSprite = loadBackgroundSprite(&backgroundTexture, "shop");
	backgroundSprite.setPosition(0, 0);
	changeButton = Button(sf::Vector2f(window->getSize().x / 2 - 100,
		window->getSize().y - 70), sf::Vector2f(200, 50), "-}");
	nextPlayer = Button(sf::Vector2f(window->getSize().x / 2 + 150,
		window->getSize().y - 70), sf::Vector2f(200, 50), "next player");
	wallIcon.setPosition(20, window->getSize().y / 2);
	interface1 = new Gui(window);
	shopCards = new ShopCards();
}

Shop::~Shop()
{
	delete interface1;
	delete shopCards;
}

void Shop::start()
{
	shopCards->updateDecks();
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
	int price = shopCards->getPriceOfCard(cardNum);
	if (remainingGold < price )
	{
		return false;
	}

	addLastItemToStorage();

	addCard(cardNum);
	shopCards->removeCard(cardNum);
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

void Shop::addCard(int cardNum)
{
	if (shopCards->currentPage)
	{
		lastItem = shopCards->getCard<EquipmentCard>(cardNum);
	}
	else
	{
		WarriorCard* warriorCard = shopCards->getCard<WarriorCard>(cardNum);
		warriorCard->getWarrior()->setSide(currentPlayerIndex);
		shopPawns[currentPlayerIndex].addPawn(warriorCard->getWarrior());
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
		if (currentRound == 4) //should be 7 so that 6 rounds happen
		{
			startGame();
			return;
		}
	}
	std::string text = "Round: " + std::to_string(currentRound);
	roundText.setString(text);
	shopCards->updateDecks();
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
	shopCards->draw(window);
	shopStorage[currentPlayerIndex].draw(window);
	shopPawns[currentPlayerIndex].draw(window);
	drawChangeButton();
	drawNextPlayerButton();
	wallIcon.draw(*window);
	window->display();
}

void Shop::drawChangeButton()
{
	if (shopCards->currentPage)
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
			shopCards->flipPage();
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
	int cardNum = shopCards->whichCardClicked(mousePosition);
	int pawnNum = shopPawns[currentPlayerIndex].whichPawnClicked(mousePosition);
	int storageCardNum = shopStorage[currentPlayerIndex].whichItemClicked(mousePosition);
	if (cardNum != -1)
	{
		shopCards->clickCard(cardNum);
	}
	else if (changeButton.isClicked(mousePosition))
	{
		shopCards->flipPage();
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
	int cardNum = shopCards->whichCardClicked(mousePosition);
	if (cardNum != -1)
	{
		buy(cardNum);
	}
	else if (wallIcon.isClicked(mousePosition))
	{
		buyWall();
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
	shopCards->unClickAll();
}

void Shop::startGame()
{
	interface1->addPawns(shopPawns[0].getPawns(), 0);
	interface1->addPawns(shopPawns[1].getPawns(), 1);
	interface1->start();
}
