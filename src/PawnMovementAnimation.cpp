#include "PawnMovementAnimation.h"

#include <algorithm>
#include <cmath>

void PawnMovementAnimation::start(const std::vector<sf::Vector2f>& waypoints, float speed)
{
    points = waypoints;
    this->speed = speed;
    segmentIndex = 0;
    segmentProgress = 0.0f;
    active = points.size() >= 2;
}

void PawnMovementAnimation::update(float dt)
{
    if (!active || points.size() < 2) {
        return;
    }

    float remainingDistance = speed * dt;
    while (active && segmentIndex < static_cast<int>(points.size()) - 1 && remainingDistance > 0.0f) {
        const sf::Vector2f& from = points[segmentIndex];
        const sf::Vector2f& to = points[segmentIndex + 1];
        const sf::Vector2f segment = to - from;
        const float segmentLength = std::hypot(segment.x, segment.y);

        if (segmentLength < 0.001f) {
            segmentIndex++;
            segmentProgress = 0.0f;
            continue;
        }

        const float distanceLeft = segmentLength * (1.0f - segmentProgress);
        const bool isLastSegment = segmentIndex + 1 >= static_cast<int>(points.size()) - 1;
        if (remainingDistance >= distanceLeft) {
            remainingDistance -= distanceLeft;
            if (isLastSegment) {
                segmentProgress = 1.0f;
                active = false;
                remainingDistance = 0.0f;
            }
            else {
                segmentIndex++;
                segmentProgress = 0.0f;
            }
        }
        else {
            segmentProgress += remainingDistance / segmentLength;
            remainingDistance = 0.0f;
            if (isLastSegment && segmentProgress >= 1.0f) {
                segmentProgress = 1.0f;
                active = false;
            }
        }
    }
}

bool PawnMovementAnimation::isActive() const
{
    return active;
}

float PawnMovementAnimation::getFacingAngle() const
{
    if (points.size() < 2) {
        return 0.0f;
    }

    int segment = segmentIndex;
    if (!active) {
        segment = static_cast<int>(points.size()) - 2;
    }
    segment = std::max(0, std::min(segment, static_cast<int>(points.size()) - 2));

    const sf::Vector2f segmentVector = points[segment + 1] - points[segment];
    if (std::hypot(segmentVector.x, segmentVector.y) < 0.001f) {
        return 0.0f;
    }

    constexpr float kRadToDeg = 180.0f / 3.14159265f;
    return std::atan2(segmentVector.y, segmentVector.x) * kRadToDeg + kSpriteForwardOffset;
}

sf::Vector2f PawnMovementAnimation::getPosition() const
{
    if (points.empty()) {
        return {};
    }
    if (!active) {
        return points.back();
    }
    if (segmentIndex >= static_cast<int>(points.size()) - 1) {
        return points.back();
    }

    const sf::Vector2f& from = points[segmentIndex];
    const sf::Vector2f& to = points[segmentIndex + 1];
    return from + (to - from) * segmentProgress;
}
