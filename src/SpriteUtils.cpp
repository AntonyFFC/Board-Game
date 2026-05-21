#include "SpriteUtils.h"

void setScalSpriteMap(float scl, std::map<std::string, sf::Sprite>& spriteMap)
{
    for (auto& pair : spriteMap)
    {
        sf::Sprite& currentSprite = pair.second;
        currentSprite.setScale(scl, scl);
    }
}

void setPosSpriteMap(int x, int y, std::map<std::string, sf::Sprite>& spriteMap)
{
    for (auto& pair : spriteMap)
    {
        sf::Sprite& currentSprite = pair.second;
        currentSprite.setPosition(x, y);
    }
}

void moveSpriteMap(int addx, int addy, std::map<std::string, sf::Sprite>& spriteMap)
{
    for (auto& pair : spriteMap)
    {
        sf::Sprite& currentSprite = pair.second;
        currentSprite.move(addx, addy);
    }
}

sf::Sprite loadBackgroundSprite(sf::Texture* backgroundTexture, std::string name)
{
    sf::Sprite newSprite;
    if (!backgroundTexture->loadFromFile("assets/backgrounds/"+name+".png")) {
        throw std::runtime_error("Unable to load background");
    }

    newSprite = sf::Sprite(*backgroundTexture);
    return newSprite;
}
