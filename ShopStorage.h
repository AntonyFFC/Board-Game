#pragma once
#include <SFML/Graphics.hpp>
#include "Card.h"
class ShopStorage
{
public:
	ShopStorage();

	void draw(sf::RenderTarget* target);
	int whichItemClicked(sf::Vector2i mousePosition);
	void addCard(EquipmentCard* itemCard);
	EquipmentCard* takeItem(int index);
private:
	void removeItem(int index);
	sf::RectangleShape rectangle;
	std::vector<EquipmentCard*> storedItems;
};

