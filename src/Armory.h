#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "Equipment.h"
#include "Pawn.h"
#include "EquipmentManager.h"
#include "PawnsManager.h"
#include "Globals.h"
#include "SpriteUtils.h"
#include "TableUtils.h"
#include "Button.h"

class Armory
{
public:
	Armory();
	~Armory();

	void rebuildLayout(const sf::FloatRect& root);
	void handleEvent(const sf::Event& event, sf::Vector2f logicalMouse);
	void update(sf::Vector2f logicalMouse, sf::RenderWindow* cursorWindow);
	void draw(sf::RenderTarget& target);

	bool wantsExit() const { return wantsExit_; }
	void resetExit() { wantsExit_ = false; }

private:
	void createEquipmentTexture();
	void createPawnsTexture();
	void drawHeaders(char which);
	void drawTitleText(char which);
	void drawEquipmentHeaders();
	void drawPawnsHeaders();
	void drawEquipment();
	void drawPawns();
	void initializeEquipmentTable();
	void initializePawnsTable();
	void flipPage();

	std::vector<Equipment*> equipmentList;
	std::vector<Pawn*> pawnsList;
	sf::RectangleShape cell;
	sf::Text text;
	sf::Text titleText;
	std::map<std::string, sf::Sprite> iconSprites;
	std::vector<sf::Texture*> iconTextures;
	sf::Sprite backgroundSprite;
	sf::Texture backgroundTexture;
	std::string equipmentHeaders[8];
	std::string pawnHeaders[7];
	std::vector<std::function<std::string(const Equipment&)>> equipmentFunctions;
	std::vector<std::function<std::string(const Pawn&)>> pawnFunctions;
	int equipmentCellWidths[8];
	int pawnCellWidths[7];
	int sumOfEqCellWidths;
	int sumOfPnCellWidths;
	int cellHeight;
	int fontSize;
	sf::Vector2f position;
	sf::RenderTexture equipmentRenderTexture;
	sf::RenderTexture pawnsRenderTexture;
	sf::Sprite equipmentTableSprite;
	sf::Sprite pawnsTableSprite;
	bool wantsExit_ = false;
	bool isPawnsShown = false;
	bool tablesBuilt_ = false;
	Button backButton;
	Button changeButton;
	sf::FloatRect root_;
	sf::FloatRect tableArea_;
};
