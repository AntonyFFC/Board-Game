#pragma once
#include <SFML/Graphics.hpp>
#include "Card.h"
class ShopStorage
{
public:
	ShopStorage();

	void draw(sf::RenderTarget* target);
	int whichItemClicked(sf::Vector2i mousePosition);
	bool isClicked(sf::Vector2i mousePosition);
	void addCard(EquipmentCard* itemCard);
	EquipmentCard* takeItem(int index);
	void addWall();
private:
	void drawRectangle(sf::RenderTarget* target);
	void drawCards(sf::RenderTarget* target);
	void drawWalls(sf::RenderTarget* target);

	void removeItem(int index);
	sf::RectangleShape rectangle;
	float radius;
	sf::CircleShape circle;
	std::vector<EquipmentCard*> storedItems;
	int numberOfWalls;
};

