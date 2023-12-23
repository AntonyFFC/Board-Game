#include "ShopCards.h"

ShopCards::ShopCards()
{
	currentPage = 0;
	equipmentList = EquipmentManager::loadEquipmentFromJson("equipment");
	pawnsList = PawnsManager::loadPawnsFromJson("pawns");
}

ShopCards::~ShopCards()
{
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

void ShopCards::draw(sf::RenderTarget* window)
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

void ShopCards::updateDecks()
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
	makeCards(availableItems, availableWarriors);
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
