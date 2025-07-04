#include "Button.h"
#include "Globals.h"

std::vector<Button*> Button::allButtons;

Button::Button(sf::Vector2f position, sf::Vector2f size, std::string text)
{
    initializeFont();
	backgroundColor = sf::Color::Transparent;
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
    buttonShape.setFillColor(backgroundColor); // Background color
    buttonShape.setOutlineColor(sf::Color::White); // Outline color
    buttonShape.setOutlineThickness(1.f); // Outline thickness

	allButtons.push_back(this); // Add this button to the static vector of all buttons
}

Button::~Button()
{
	allButtons.erase(std::remove(allButtons.begin(), allButtons.end(), this), allButtons.end());
    // No dynamic memory to clean up
}

bool Button::click(sf::Vector2i mousePosition)
{
    if (isClicked(mousePosition))
    {
        setIsBeingClicked(true);
        return true;
    }
    return false;
}

bool Button::unclick()
{
    if (!isBeingClicked)
    {
        return false;
    }
	setIsBeingClicked(false);
	return true;
}

bool Button::unclickAll()
{
    bool wasBeingClicked = isBeingClicked;
	for (Button* button : allButtons)
	{
		button->setIsBeingClicked(false);
	}
    return wasBeingClicked;
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

void Button::setSizeToText()
{
	sf::FloatRect textBounds = buttonText.getLocalBounds();
	buttonShape.setSize(sf::Vector2f(textBounds.width * 1.2f, textBounds.height * 1.2f));
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

void Button::setTextSize(unsigned int size)
{
	buttonText.setCharacterSize(size);
	setTextPosition();
}

void Button::setBackgroundColor(sf::Color color)
{
	backgroundColor = color;
    buttonShape.setFillColor(backgroundColor);
}

void Button::setOutlineColor(sf::Color color)
{
    buttonShape.setOutlineColor(color);
}

void Button::setOutlineThickness(float thickness)
{
    buttonShape.setOutlineThickness(thickness);
}

void Button::setIsBeingClicked(bool boolean)
{
	isBeingClicked = boolean;
	if (boolean)
	{
		buttonShape.setFillColor(sf::Color::Cyan);
	}
	else
	{
		buttonShape.setFillColor(backgroundColor);
	}
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
