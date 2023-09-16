#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "Pawn.h"
#include "Equipment.h"
#include "SpriteUtils.h"
#include "TableUtils.h"
#include "Globals.h"
class Card
{
public:
	friend class WarriorCard;
	friend class EquipmentCard;

	Card(const std::vector<int>& widths, const std::vector<std::string>& headers);
	~Card();

	template <typename RenderType>
	void draw(RenderType* window);
	sf::Sprite getSprite();

	bool isClicked(sf::Vector2i mousePosition);
private:
	sf::Sprite loadSprite(const std::string& textureName);
	void createSprite();
	void createTexture();
	virtual void drawValues();
	void drawHeaders();
	virtual void drawPicture();
	sf::Vector2f position;
	int fontSize;
	int cellHeight;
	int sumOfCellWidths;
	sf::Texture texture;
	sf::Sprite sprite;
	sf::RenderTexture renderTexture;
	sf::RectangleShape cell;
	sf::Text text;
	std::vector<int> cellWidths;
	std::vector<std::string> headers;
	std::map<std::string, sf::Sprite> iconSprites;
	std::vector<sf::Texture*> iconTextures;
};

class WarriorCard : public Card {
public:
    // Constructor
    WarriorCard(Pawn* warrior);

    // Destructor
    ~WarriorCard();

private:
	void drawValues() override;
	void drawPicture() override;
    Pawn* warrior;
	std::vector<std::function<std::string(const Pawn&)>> functions;
};

class EquipmentCard : public Card {
public:
    // Constructor
    EquipmentCard(Equipment* item);

    // Destructor
    ~EquipmentCard();

private:
	void drawValues() override;
	void drawPicture() override;
    Equipment* item;
	std::vector<std::function<std::string(const Equipment&)>> functions;
};
