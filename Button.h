#pragma once
#include <SFML/Graphics.hpp>

class Button
{
public:
    // Constructor
    Button(sf::Vector2f position = sf::Vector2f(0, 0), sf::Vector2f size = sf::Vector2f(100, 30), std::string text = "Button");
    ~Button();

    bool isClicked(sf::Vector2i mousePosition);
    void setPosition(sf::Vector2f position);
    sf::Vector2f getPosition() const;
    void setSize(sf::Vector2f size);
    sf::Vector2f getSize() const;
    void setText(std::string text);
    std::string getText() const;
    void setFont(sf::Font& font);
    void setTextColor(sf::Color color);
    void setBackgroundColor(sf::Color color);
    void setOutlineColor(sf::Color color);
    void setOutlineThickness(float thickness);
    void draw(sf::RenderTexture& window);
    void draw(sf::RenderWindow& window);

private:
    void setTextPosition();
    sf::RectangleShape buttonShape;
    sf::Text buttonText;
};
