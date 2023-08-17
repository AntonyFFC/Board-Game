#pragma once

#include <SFML/Graphics.hpp>
#include <map>

void setScalSpriteMap(float scl, std::map<std::string, sf::Sprite>& spriteMap);
void setPosSpriteMap(int x, int y, std::map<std::string, sf::Sprite>& spriteMap);
void moveSpriteMap(int addx, int addy, std::map<std::string, sf::Sprite>& spriteMap);


