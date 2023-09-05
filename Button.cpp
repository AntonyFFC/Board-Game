#include "Button.h"
#include "Globals.h"

Button::Button(sf::Vector2f position, sf::Vector2f size, std::string text)
{
    initializeFont();
    // Set the position and size of the button shape
    buttonShape.setPosition(position);
    buttonShape.setSize(size);

    // Initialize text attributes
    buttonText.setString(text);
    buttonText.setFont(globalFont2); // You can set the font here
    buttonText.setCharacterSize(30); // Set the character size of the text
    buttonText.setFillColor(sf::Color::White); // Set the default text color
    setTextPosition();

    // Default colors for button
    buttonShape.setFillColor(sf::Color::Transparent); // Background color
    buttonShape.setOutlineColor(sf::Color::White); // Outline color
    buttonShape.setOutlineThickness(1.f); // Outline thickness
}

Button::~Button()
{
    // No dynamic memory to clean up
}

bool Button::isClicked(sf::Vector2i mousePosition)
{
    sf::FloatRect buttonBounds = buttonShape.getGlobalBounds();
    return buttonBounds.contains(static_cast<sf::Vector2f>(mousePosition));
}

void Button::setPosition(sf::Vector2f position)
{
    buttonShape.setPosition(position);
    setTextPosition();
}

sf::Vector2f Button::getPosition() const
{
    return buttonShape.getPosition();
}

void Button::setSize(sf::Vector2f size)
{
    buttonShape.setSize(size);
}

sf::Vector2f Button::getSize() const
{
    return buttonShape.getSize();
}

void Button::setText(std::string text)
{
    buttonText.setString(text);
    setTextPosition();
}

std::string Button::getText() const
{
    return buttonText.getString();
}

void Button::setFont(sf::Font& font)
{
    buttonText.setFont(font);
    setTextPosition();
}

void Button::setTextColor(sf::Color color)
{
    buttonText.setFillColor(color);
}

void Button::setBackgroundColor(sf::Color color)
{
    buttonShape.setFillColor(color);
}

void Button::setOutlineColor(sf::Color color)
{
    buttonShape.setOutlineColor(color);
}

void Button::setOutlineThickness(float thickness)
{
    buttonShape.setOutlineThickness(thickness);
}

void Button::draw(sf::RenderTexture& window)
{
    window.draw(buttonShape);
    window.draw(buttonText);
}

void Button::draw(sf::RenderWindow& window)
{
    window.draw(buttonShape);
    window.draw(buttonText);
}

void Button::setTextPosition()
{
    sf::FloatRect textBounds = buttonText.getLocalBounds();
    float xPosition = buttonShape.getPosition().x + buttonShape.getSize().x / 2 - textBounds.width / 2;
    float yPosition = buttonShape.getPosition().y + buttonShape.getSize().y / 2.0f - textBounds.height / 2.0f;
    buttonText.setPosition(xPosition, yPosition);
}
