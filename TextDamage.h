#pragma once
#include <SFML/Graphics.hpp>
#include "Globals.h"
class TextDamage
{
public:
    TextDamage(const std::string& text, sf::Vector2f position, float duration = 1.0f);

    void update(float dt);
    void draw(sf::RenderTarget& target) const;
    bool isStillSeen() const;

private:
    sf::Text floatText;
    float lifetime;
    float elapsed;
    bool isSeen;
};

