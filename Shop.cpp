#include "Shop.h"

Shop::Shop(sf::RenderWindow* window)
	:window(window)
{
	initializeFont();
	currentRound = 0;
	currentPlayerIndex = 0;
	remainingGold = 6;
}

void Shop::displayShop()
{
	window->clear(sf::Color(71, 31, 16));
	window->draw(backgroundSprite);
	for (Card& card : availableCards)
	{
		window->draw(card.getSprite());
	}
	window->display();
}
