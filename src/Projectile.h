#pragma once

#include <SFML/Graphics.hpp>

class Projectile
{
public:
    Projectile(const sf::Texture& texture, sf::Vector2f start, sf::Vector2f target,
        float flightDuration = 0.4f, float baseScale = 0.06f, float maxArcHeight = 32.0f);

    void update(float dt);
    bool isFinished() const;
    void draw(sf::RenderTarget& target) const;

private:
    static float ease(float t);
    sf::Vector2f positionAt(float easedT) const;
    sf::Vector2f tangentAt(float easedT) const;
    void applyPose(float easedT);

    sf::Sprite sprite;
    sf::Vector2f startPos;
    sf::Vector2f targetPos;
    float elapsed = 0.0f;
    float flightDuration;
    float baseScale;
    float maxArcHeight;
    bool finished = false;
};
