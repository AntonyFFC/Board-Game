#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "Pawn.h"
#include "Equipment.h"
class Card
{
public:
	friend class WarriorCard;
	friend class EquipmentCard;

	Card();
	~Card();

	template <typename RenderType>
	void draw(RenderType& window);

	bool isClicked(sf::Vector2i mousePosition);
private:
	void createSprite();
	sf::Vector2f position;
	sf::Sprite sprite;
	sf::Texture texture;
	sf::RectangleShape cell;
	sf::Text text;
};

class WarriorCard : public Card {
public:
    // Constructor
    WarriorCard(Pawn* warrior);

    // Destructor
    ~WarriorCard();


private:
    // Private member variables
    Pawn* warrior;
};

class EquipmentCard : public Card {
public:
    // Constructor
    EquipmentCard(Equipment* item);

    // Destructor
    ~EquipmentCard();


private:
    Equipment* item;
};
