#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

class Button
{
public:
    Button(sf::Vector2f position = sf::Vector2f(0, 0), sf::Vector2f size = sf::Vector2f(100, 30), std::string text = "Button");
    ~Button();
    Button(const Button&) = delete;
    Button& operator=(const Button&) = delete;

	bool click(sf::Vector2i mousePosition);
	bool unclick();
    bool unclickAll();
    bool isClicked(sf::Vector2i mousePosition) const;
    void setPosition(sf::Vector2f position);
    sf::Vector2f getPosition() const;
    void setSize(sf::Vector2f size);
	void setSizeToText();
    sf::Vector2f getSize() const;
    void setText(std::string text);
    std::string getText() const;
    void setFont(sf::Font& font);
    void setTextColor(sf::Color color);
	void setTextSize(unsigned int size);
    void setBackgroundColor(sf::Color color);
    void setOutlineColor(sf::Color color);
    void setOutlineThickness(float thickness);
	void setIsBeingClicked(bool boolean);
    void draw(sf::RenderTexture& window);
    void draw(sf::RenderWindow& window);

    static void updateAll(sf::Vector2i mousePosition, sf::RenderWindow* window = nullptr);
    static void resetCursor(sf::RenderWindow* window);

private:
    static sf::ConvexShape makeRoundedRect(sf::Vector2f size, float radius);
    static sf::Color brighten(sf::Color color, float factor);
    static sf::Color darken(sf::Color color, float factor);
    void rebuildShapes();
    void applyVisualState();
    void setTextPosition();
    sf::Color getBaseFillColor() const;
    sf::Color getBaseBorderColor() const;

    sf::Vector2f buttonPosition;
    sf::Vector2f buttonSize;
    sf::ConvexShape shadowShape;
    sf::ConvexShape buttonShape;
    sf::Text buttonText;
	bool isBeingClicked = false;
    bool isHovered = false;
    bool hasCustomBaseColor = false;
	static std::vector<Button*> allButtons;
	sf::Color customBaseColor;
    sf::Color customBorderColor;
    bool hasCustomBorderColor = false;
    float cornerRadius = 8.0f;
    float outlineThickness = 2.0f;
};
