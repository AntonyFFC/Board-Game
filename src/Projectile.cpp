#include "Projectile.h"

#include <cmath>

namespace {
    constexpr float kRadToDeg = 180.0f / 3.14159265f;
    constexpr float kApexScaleBoost = 0.35f;
    constexpr float kTangentDelta = 0.01f;
}

Projectile::Projectile(const sf::Texture& texture, sf::Vector2f start, sf::Vector2f target,
    float flightDuration, float baseScale, float maxArcHeight)
    : sprite(texture), startPos(start), targetPos(target),
    flightDuration(flightDuration), baseScale(baseScale), maxArcHeight(maxArcHeight)
{
    if (flightDuration < 0.001f) {
        finished = true;
    }

    sprite.setOrigin(sprite.getLocalBounds().width / 2.0f,
        sprite.getLocalBounds().height / 2.0f);
    applyPose(0.0f);
}

float Projectile::ease(float t)
{
    t = std::max(0.0f, std::min(1.0f, t));
    return t * t * (3.0f - 2.0f * t);
}

sf::Vector2f Projectile::positionAt(float easedT) const
{
    const sf::Vector2f ground = startPos + (targetPos - startPos) * easedT;
    const float height = 4.0f * maxArcHeight * easedT * (1.0f - easedT);
    return sf::Vector2f(ground.x, ground.y - height);
}

sf::Vector2f Projectile::tangentAt(float easedT) const
{
    const float t0 = std::max(0.0f, easedT - kTangentDelta);
    const float t1 = std::min(1.0f, easedT + kTangentDelta);
    const sf::Vector2f p0 = positionAt(t0);
    const sf::Vector2f p1 = positionAt(t1);
    return p1 - p0;
}

void Projectile::applyPose(float easedT)
{
    const sf::Vector2f pos = positionAt(easedT);
    const float height = 4.0f * maxArcHeight * easedT * (1.0f - easedT);
    const float heightFactor = maxArcHeight > 0.001f ? height / maxArcHeight : 0.0f;
    const float currentScale = baseScale * (1.0f + kApexScaleBoost * heightFactor);

    sprite.setPosition(pos);
    sprite.setScale(currentScale, currentScale);

    const sf::Vector2f tangent = tangentAt(easedT);
    if (std::hypot(tangent.x, tangent.y) > 0.001f) {
        sprite.setRotation(std::atan2(tangent.y, tangent.x) * kRadToDeg);
    }
}

void Projectile::update(float dt)
{
    if (finished || flightDuration < 0.001f) {
        return;
    }

    elapsed += dt;
    const float rawT = std::min(1.0f, elapsed / flightDuration);
    const float easedT = ease(rawT);
    applyPose(easedT);

    if (rawT >= 1.0f) {
        applyPose(1.0f);
        finished = true;
    }
}

bool Projectile::isFinished() const
{
    return finished;
}

void Projectile::draw(sf::RenderTarget& target) const
{
    target.draw(sprite);
}
