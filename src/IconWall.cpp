#include "iconWall.h"
#include "Globals.h"

// Constructor
IconWall::IconWall(float x, float y)
{
    radius = 30.f;
    initializeFont();
    // Initialize the icon shape (brown circular icon)
    iconShape.setRadius(radius);
    iconShape.setPosition(x, y);
    setIsBeingClicked(false);
    // Initialize the text
    iconText.setString("1$");
    iconText.setFont(globalFont2);
    iconText.setCharacterSize(radius);
    iconText.setFillColor(sf::Color::White);
    iconText.setPosition(x + radius / 2, y + radius / 2);
}

// Draw the icon on the given render target
void IconWall::draw(sf::RenderWindow& window)
{
    window.draw(iconShape);
    window.draw(iconText);
}

// Check if the icon is clicked
bool IconWall::isClicked(sf::Vector2i mousePosition)
{
    sf::FloatRect iconBounds = iconShape.getGlobalBounds();
    return iconBounds.contains(static_cast<sf::Vector2f>(mousePosition));
}

// Set the position of the icon
void IconWall::setPosition(float x, float y)
{
    iconShape.setPosition(x, y);
    iconText.setPosition(x + radius / 2, y + radius / 2);
}
void IconWall::setIsBeingClicked(bool boolean)
{
    isBeingClicked = boolean;
    if (boolean)
    {
        iconShape.setFillColor(sf::Color(138, 94, 62));
    }
    else
    {
        iconShape.setFillColor(sf::Color(139, 69, 19));
    }
}