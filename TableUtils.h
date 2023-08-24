#pragma once
#define NOMINMAX
#include <Windows.h>
#include <unordered_set>
#include <stdexcept>
#include "Globals.h"
#include <functional>
#include "Equipment.h"

void drawSpaceIcon(Equipment::SpaceOccupied space, sf::RenderWindow* target, std::map<std::string, sf::Sprite>& iconSprites);
void drawTypeIcon(std::string type, sf::RenderWindow* target, std::map<std::string, sf::Sprite>& iconSprites);

std::map<std::string, sf::Sprite> initializeSpriteMap(std::vector<sf::Texture*> &iconTextures);
std::vector<std::function<std::string(const Equipment&)>> initializeFunctions();
sf::RectangleShape initializeCells();