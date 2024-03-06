#include "ShopCards.h"

ShopCards::ShopCards()
{
	currentPage = 0;
	equipmentList = EquipmentManager::loadEquipmentFromJson("equipment");
	pawnsList = PawnsManager::loadPawnsFromJson("pawns");
	implicateNumInDeck();

	sortEquipment(equipmentList);
	sortWarriors(pawnsList);
}

ShopCards::~ShopCards()
{
	for (WarriorCard* warriorCard : warriorsCards)
	{
		delete warriorCard;
	}
	warriorsCards.clear();
	pawnsList.clear();
	for (EquipmentCard* itemCard : itemsCards)
	{
		delete itemCard;
	}
	itemsCards.clear();
	equipmentList.clear();
}

void ShopCards::draw(sf::RenderTarget* window)
{
	if (currentPage)
	{
		for (auto& itemCard : itemsCards)
		{
			window->draw(itemCard->getFullSprite());
		}
	}
	else
	{
		for (auto& warriorCard : warriorsCards)
		{
			window->draw(warriorCard->getFullSprite());
		}
	}
}

void ShopCards::setPositions(sf::RenderTarget* window)
{
	sf::Vector2f pos(window->getSize().x / 4, 20);
	for (auto& itemCard : itemsCards)
	{
		itemCard->setScale(0.8);
		itemCard->setPosition(sf::Vector2f(pos.x - itemCard->getSprite().getGlobalBounds().width / 2, pos.y));
		pos += sf::Vector2f(0, itemCard->getSprite().getGlobalBounds().height + 5);
	}
	pos = sf::Vector2f(window->getSize().x / 4, 20);
	for (auto& warriorCard : warriorsCards)
	{
		warriorCard->setScale(0.75);
		warriorCard->setPosition(sf::Vector2f(pos.x - warriorCard->getSprite().getGlobalBounds().width / 2, pos.y));
		pos += sf::Vector2f(0, warriorCard->getSprite().getGlobalBounds().height + 5);
	}
}

void ShopCards::makeCards(std::vector<Equipment*> availableItems, std::vector<Pawn*> availableWarriors)
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

void ShopCards::updateDecks(sf::RenderTarget* window)
{
	std::vector<Equipment*> availableItems;
	std::vector<Pawn*> availableWarriors;

	// Shuffle the lists
	std::random_device rd;
	std::mt19937 gen(rd());
	std::shuffle(expensivePawnsList.begin(), expensivePawnsList.end(), gen);
	std::shuffle(cheapPawnsList.begin(), cheapPawnsList.end(), gen);

	// Select 2 armours, 3 weapons, and 2 accessories randomly
	availableWarriors.clear();
	availableWarriors.insert(availableWarriors.end(), expensivePawnsList.begin(), expensivePawnsList.begin() + 2);
	availableWarriors.insert(availableWarriors.end(), cheapPawnsList.begin(), cheapPawnsList.begin() + 3);

	std::shuffle(armourList.begin(), armourList.end(), gen);
	std::shuffle(weaponsList.begin(), weaponsList.end(), gen);
	std::shuffle(accesoriesList.begin(), accesoriesList.end(), gen);

	// Select 2 armours, 3 weapons, and 2 accessories randomly
	availableItems.clear();
	availableItems.insert(availableItems.end(), armourList.begin(), armourList.begin() + 2);
	availableItems.insert(availableItems.end(), weaponsList.begin(), weaponsList.begin() + 3);
	availableItems.insert(availableItems.end(), accesoriesList.begin(), accesoriesList.begin() + 2);
	makeCards(availableItems, availableWarriors);
	setPositions(window);
}

int ShopCards::whichCardClicked(sf::Vector2i mousePosition)
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

int ShopCards::getPriceOfCard(int cardNum)
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

void ShopCards::clickCard(int cardNum)
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

void ShopCards::unClickAll()
{
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

void ShopCards::flipPage()
{
	currentPage = !currentPage;
}

void ShopCards::removeCard(int cardNum)
{
	if (currentPage)
	{
		Equipment* item = itemsCards[cardNum]->getItem();
		std::string type = item->getType();
		if (type == "Weapon") {
			weaponsList.erase(std::find(weaponsList.begin(), weaponsList.end(), item));
		}
		else if (type == "Armour") {
			armourList.erase(std::find(armourList.begin(), armourList.end(), item));
		}
		else if (type == "Accesory") {
			accesoriesList.erase(std::find(accesoriesList.begin(), accesoriesList.end(), item));
		}
		else {
			// Handle unexpected types if needed
		}
		//here I remove from the cards, which are in the shop
		itemsCards.erase(itemsCards.begin() + cardNum);
	}
	else
	{
		Pawn* warrior = warriorsCards[cardNum]->getWarrior();
		int price = warrior->getPrice();
		std::string name = warrior->getName();
		if (price < 4 || name == "Townsman Defender")
		{
			cheapPawnsList.erase(std::find(cheapPawnsList.begin(), cheapPawnsList.end(), warrior));
		}
		else
		{
			expensivePawnsList.erase(std::find(expensivePawnsList.begin(), expensivePawnsList.end(), warrior));
		}
		//here I remove from the cards, which are in the shop
		warriorsCards.erase(warriorsCards.begin() + cardNum);
	}
}

void ShopCards::sortEquipment(std::vector<Equipment*>& equipmentList)
{
	for (Equipment* equip : equipmentList)
	{
		std::string type = equip->getType();
		if (type == "Weapon") {
			weaponsList.push_back(equip);
		}
		else if (type == "Armour") {
			armourList.push_back(equip);
		}
		else if (type == "Accesory") {
			accesoriesList.push_back(equip);
		}
		else {
			// Handle unexpected types if needed
		}
	}
}

void ShopCards::sortWarriors(std::vector<Pawn*>& pawnsList)
{
	for (Pawn* pawn : pawnsList)
	{
		int price = pawn->getPrice();
		std::string name = pawn->getName();
		if (price < 4 || name == "Townsman Defender")
		{
			cheapPawnsList.push_back(pawn);
		}
		else
		{
			expensivePawnsList.push_back(pawn);
		}
	}
}

void ShopCards::implicateNumInDeck()
{
	std::vector<Equipment*> replicatedListEq;

	for (Equipment* item : equipmentList) {
		for (int i = 0; i < item->getNumInDeck(); ++i) {
			replicatedListEq.push_back(item);
		}
	}
	equipmentList = replicatedListEq;

	std::vector<Pawn*> replicatedListPw;

	for (Pawn* item : pawnsList) {
		for (int i = 0; i < item->getNumInDeck(); ++i) {
			replicatedListPw.push_back(item);
		}
	}
	pawnsList = replicatedListPw;
}
