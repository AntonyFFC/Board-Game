#pragma once

#include <SFML/Graphics.hpp>

class Projectile
{
public:
    Projectile(const sf::Texture& texture, sf::Vector2f start, sf::Vector2f target,
        float speed = 400.0f, float scale = 0.07f);

    void update(float dt);
    bool isFinished() const;
    void draw(sf::RenderTarget& target) const;

private:
    sf::Sprite sprite;
    sf::Vector2f startPos;
    sf::Vector2f targetPos;
    float progress = 0.0f;
    float speed = 400.0f;
    float totalDistance = 0.0f;
    bool finished = false;
};
