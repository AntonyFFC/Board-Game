#pragma once
#include <SFML/Graphics.hpp>

class IconWall
{
public:
    // Constructor
    IconWall(float x = 0.f, float y = 0.f);

    // Draw the icon on the given render target
    void draw(sf::RenderWindow& window);

    // Check if the icon is clicked
    bool isClicked(sf::Vector2i mousePosition);

    // Set the position of the icon
    void setPosition(float x, float y);
    // Set the clicked state of the icon
    void setIsBeingClicked(bool boolean);
    
    sf::CircleShape iconShape;
    sf::Text iconText;
    bool isBeingClicked;

private:
    // Private member variables
    float radius;
};

