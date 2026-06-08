#include "Projectile.h"

#include <cmath>

namespace {
    constexpr float kRadToDeg = 180.0f / 3.14159265f;
}

Projectile::Projectile(const sf::Texture& texture, sf::Vector2f start, sf::Vector2f target,
    float speed, float scale)
    : sprite(texture), startPos(start), targetPos(target), speed(speed)
{
    const sf::Vector2f delta = targetPos - startPos;
    totalDistance = std::hypot(delta.x, delta.y);
    if (totalDistance < 0.001f) {
        finished = true;
        progress = 1.0f;
    }

    sprite.setOrigin(sprite.getLocalBounds().width / 2.0f,
        sprite.getLocalBounds().height / 2.0f);
    sprite.setScale(scale, scale);
    sprite.setPosition(startPos);

    const float angle = std::atan2(delta.y, delta.x) * kRadToDeg;
    sprite.setRotation(angle);
}

void Projectile::update(float dt)
{
    if (finished || totalDistance < 0.001f) {
        return;
    }

    progress += (speed * dt) / totalDistance;
    if (progress >= 1.0f) {
        progress = 1.0f;
        finished = true;
    }

    sprite.setPosition(startPos + (targetPos - startPos) * progress);
}

bool Projectile::isFinished() const
{
    return finished;
}

void Projectile::draw(sf::RenderTarget& target) const
{
    target.draw(sprite);
}
