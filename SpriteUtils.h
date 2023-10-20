#pragma once

#include <SFML/Graphics.hpp>
#include <map>

// iconSpriteMaps functions
void setScalSpriteMap(float scl, std::map<std::string, sf::Sprite>& spriteMap);
void setPosSpriteMap(int x, int y, std::map<std::string, sf::Sprite>& spriteMap);
void moveSpriteMap(int addx, int addy, std::map<std::string, sf::Sprite>& spriteMap);

//background functions
sf::Sprite loadBackgroundSprite(sf::Texture* backgroundTexture, std::string name);


