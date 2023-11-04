#pragma once
#define NOMINMAX
#include <Windows.h>
#include <unordered_set>
#include <stdexcept>
#include "Globals.h"
#include <functional>
#include "Equipment.h"
#include "Pawn.h"

void drawSpaceIcon(Equipment::SpaceOccupied space, sf::RenderTexture& target, std::map<std::string, sf::Sprite>& iconSprites);
void drawTypeIcon(std::string type, sf::RenderTexture& target, std::map<std::string, sf::Sprite>& iconSprites);
void drawSpaceIconBlack(Equipment::SpaceOccupied space, sf::RenderTexture& target, std::map<std::string, sf::Sprite>& iconSprites);
void drawTypeIconBlack(std::string type, sf::RenderTexture& target, std::map<std::string, sf::Sprite>& iconSprites);

std::map<std::string, sf::Sprite> initializeSpriteMap(std::vector<sf::Texture*> &iconTextures);
std::vector<std::function<std::string(const Equipment&)>> initializeFunctions();
std::vector<std::function<std::string(const Pawn&)>> initializePawnFunctions();
sf::RectangleShape initializeCells(float width = 1.f, float height = 1.f, float x = 1.f, float y = 1.f);

void setPosCells(int x, int y, std::vector <sf::RectangleShape*>& cells);
void setPosTexts(int x, int y, std::vector <sf::Text*>& texts);
void moveCells(int addx, int addy, std::vector <sf::RectangleShape*>& cells);
void moveTexts(int addx, int addy, std::vector <sf::Text*>& texts);
void setSizeCells(float xSiz, float ySiz, std::vector <sf::RectangleShape*>& cells);
void setFillColorCells(sf::Color color, std::vector <sf::RectangleShape*>& cells);

sf::Color getTypeColor(Equipment* item);
sf::Color getTeamColor(int team);

sf::Text initializeText(std::string content, sf::Font* font, float fontSize, sf::Color color);