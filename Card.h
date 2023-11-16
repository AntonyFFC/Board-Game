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

	Card(const std::vector<int>& widths, const std::vector<std::string>& headers, sf::Vector2f pos = sf::Vector2f(0, 0));

	template <typename RenderType>
	void draw(RenderType* window);
	sf::Sprite getSprite();

	void setPosition(sf::Vector2f pos);
	void movePosition(sf::Vector2f pos);
	void setScale(float scl);
	bool isClicked(sf::Vector2i mousePosition) const;
	virtual int getPrice() const = 0;

	static std::map<std::string, sf::Sprite> iconSprites;
	static std::vector<sf::Texture*> iconTextures;
	static const int fontSize;
	static const int cellHeight;
	static const float scaleFactor;
private:
	sf::Sprite loadSprite(const std::string& textureName);
	void createSprite();
	void createTexture();
	virtual void drawValues() = 0;
	void drawHeaders();
	void drawPicture();
	void moveBack();
	sf::Vector2f position;
	int sumOfCellWidths;
	sf::Texture pictureTexture;
	sf::Sprite cardSprite;
	sf::Sprite pictureSprite;
	sf::RenderTexture renderTexture;
	sf::RectangleShape cell;
	sf::Text text;
	std::vector<int> cellWidths;
	std::vector<std::string> headers;
};

class WarriorCard : public Card {
public:
    // Constructor
    WarriorCard(Pawn* warrior);

	int getPrice() const;
	Pawn* getWarrior() const;

private:
	void drawValues() override;
    Pawn* warrior;
	std::vector<std::function<std::string(const Pawn&)>> functions;
};

class EquipmentCard : public Card {
public:
    // Constructor
    EquipmentCard(Equipment* item);

	int getPrice() const;
	Equipment* getItem() const;

private:
	void drawValues() override;
    Equipment* item;
	std::vector<std::function<std::string(const Equipment&)>> functions;
};
