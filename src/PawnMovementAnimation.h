#pragma once

#include <SFML/System/Vector2.hpp>
#include <vector>

class PawnMovementAnimation
{
public:
    void start(const std::vector<sf::Vector2f>& waypoints, float speed = 280.0f);
    void update(float dt);
    bool isActive() const;
    sf::Vector2f getPosition() const;

private:
    std::vector<sf::Vector2f> points;
    int segmentIndex = 0;
    float segmentProgress = 0.0f;
    float speed = 280.0f;
    bool active = false;
};
