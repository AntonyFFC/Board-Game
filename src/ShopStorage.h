#pragma once
#include <SFML/Graphics.hpp>
#include "Card.h"

class ShopStorage
{
public:
	ShopStorage();

	void setPanel(const sf::FloatRect& panel);
	void draw(sf::RenderTarget* target);
	int whichItemClicked(sf::Vector2f mousePosition);
	bool isClicked(sf::Vector2f mousePosition);
	void addCard(EquipmentCard* itemCard);
	EquipmentCard* takeItem(int index);
	void addWall();
	int getNumberOfWalls();
private:
	void drawRectangle(sf::RenderTarget* target);
	void drawCards(sf::RenderTarget* target);
	void drawWalls(sf::RenderTarget* target);

	void removeItem(int index);
	sf::RectangleShape rectangle;
	sf::FloatRect panel_;
	float radius;
	sf::CircleShape circle;
	std::vector<EquipmentCard*> storedItems;
	int numberOfWalls;
};
