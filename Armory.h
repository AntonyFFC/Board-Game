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
	Armory(sf::RenderWindow* window);
	~Armory();
	void start();
	void exit();
private:
	void display();

	void createEquipmentTexture();
	void createPawnsTexture();
	void drawHeaders(char which);
	void drawTitleText(char which);
	void drawEquipmentHeaders();
	void drawPawnsHeaders();
	void drawEquipment();
	void drawPawns();
	void drawBackButton(char which);
	void drawChangeButton(char which);
	void keyPressed(const sf::Event& event);

	void initializeText();
	void initializeEquipmentTable();
	void initializePawnsTable();
	void flipPage();
	sf::RenderWindow* window;
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
	std::string pawnHeaders[6];
	std::vector<std::function<std::string(const Equipment&)>> equipmentFunctions;
	std::vector<std::function<std::string(const Pawn&)>> pawnFunctions;
	int equipmentCellWidths[8];
	int pawnCellWidths[6];
	int sumOfEqCellWidths;
	int sumOfPnCellWidths;
	int cellHeight;
	int fontSize;
	sf::Vector2f position;
	sf::RenderTexture equipmentRenderTexture;
	sf::RenderTexture pawnsRenderTexture;
	sf::Sprite equipmentTableSprite;
	sf::Sprite pawnsTableSprite;
	bool closed;
	bool isPawnsShown;
	Button backButton;
	Button changeButton;
};
