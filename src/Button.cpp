#include "Button.h"
#include "Globals.h"
#include <algorithm>
#include <cmath>

namespace {
    const sf::Color kMedievalFill{82, 52, 34};
    const sf::Color kMedievalBorder{196, 154, 72};
    const sf::Color kMedievalText{244, 228, 196};
    const sf::Color kMedievalShadow{20, 12, 8, 120};

#if defined(SFML_VERSION_MAJOR) && (SFML_VERSION_MAJOR > 2 || (SFML_VERSION_MAJOR == 2 && SFML_VERSION_MINOR >= 5))
    const bool kSupportsCustomCursor = true;
    sf::Cursor handCursor;
    sf::Cursor arrowCursor;
    bool cursorsLoaded = false;
#else
    const bool kSupportsCustomCursor = false;
#endif
}

std::vector<Button*> Button::allButtons;

Button::Button(sf::Vector2f position, sf::Vector2f size, std::string text)
{
    initializeFont();
    customBaseColor = sf::Color::Transparent;
    customBorderColor = kMedievalBorder;

    buttonPosition = position;
    buttonSize = size;

    buttonText.setString(text);
    buttonText.setFont(globalFont2);
    buttonText.setCharacterSize(30);
    buttonText.setFillColor(kMedievalText);
    buttonText.setOutlineColor(sf::Color(35, 22, 14));
    buttonText.setOutlineThickness(1.0f);

    rebuildShapes();
    applyVisualState();
    setTextPosition();

    allButtons.push_back(this);
}

Button::~Button()
{
    allButtons.erase(std::remove(allButtons.begin(), allButtons.end(), this), allButtons.end());
}

sf::ConvexShape Button::makeRoundedRect(sf::Vector2f size, float radius)
{
    sf::ConvexShape shape;
    const float width = size.x;
    const float height = size.y;
    const float r = std::min(radius, std::min(width, height) * 0.5f);
    const unsigned int pointsPerCorner = 8;
    std::vector<sf::Vector2f> points;
    points.reserve(pointsPerCorner * 4 + 4);

    const float pi = 3.14159265f;
    const float halfPi = pi * 0.5f;

    auto addCorner = [&](float centerX, float centerY, float startAngle) {
        for (unsigned int i = 0; i <= pointsPerCorner; ++i) {
            const float angle = startAngle + halfPi * static_cast<float>(i) / static_cast<float>(pointsPerCorner);
            points.emplace_back(centerX + r * std::cos(angle), centerY + r * std::sin(angle));
        }
    };

    addCorner(r, r, pi);
    addCorner(width - r, r, pi + halfPi);
    addCorner(width - r, height - r, 0.0f);
    addCorner(r, height - r, halfPi);

    shape.setPointCount(points.size());
    for (std::size_t i = 0; i < points.size(); ++i) {
        shape.setPoint(i, points[i]);
    }
    return shape;
}

sf::Color Button::brighten(sf::Color color, float factor)
{
    auto adjust = [factor](sf::Uint8 channel) {
        return static_cast<sf::Uint8>(std::min(255.0f, channel * factor));
    };
    return sf::Color(adjust(color.r), adjust(color.g), adjust(color.b), color.a);
}

sf::Color Button::darken(sf::Color color, float factor)
{
    auto adjust = [factor](sf::Uint8 channel) {
        return static_cast<sf::Uint8>(channel * factor);
    };
    return sf::Color(adjust(color.r), adjust(color.g), adjust(color.b), color.a);
}

void Button::rebuildShapes()
{
    cornerRadius = std::min(10.0f, buttonSize.y * 0.28f);

    buttonShape = makeRoundedRect(buttonSize, cornerRadius);
    buttonShape.setPosition(buttonPosition);

    shadowShape = makeRoundedRect(buttonSize, cornerRadius);
    shadowShape.setPosition(buttonPosition + sf::Vector2f(3.0f, 3.0f));
    shadowShape.setFillColor(kMedievalShadow);
    shadowShape.setOutlineThickness(0.0f);

    applyVisualState();
}

sf::Color Button::getBaseFillColor() const
{
    if (hasCustomBaseColor) {
        return customBaseColor;
    }
    return kMedievalFill;
}

sf::Color Button::getBaseBorderColor() const
{
    if (hasCustomBorderColor) {
        return customBorderColor;
    }
    return kMedievalBorder;
}

void Button::applyVisualState()
{
    sf::Color fill = getBaseFillColor();
    sf::Color border = getBaseBorderColor();

    if (isBeingClicked) {
        fill = darken(fill, 0.72f);
        border = darken(border, 0.82f);
        buttonShape.setPosition(buttonPosition + sf::Vector2f(0.0f, 1.0f));
        shadowShape.setPosition(buttonPosition + sf::Vector2f(2.0f, 4.0f));
    }
    else if (isHovered) {
        fill = brighten(fill, 1.18f);
        border = brighten(border, 1.12f);
        buttonShape.setPosition(buttonPosition);
        shadowShape.setPosition(buttonPosition + sf::Vector2f(3.0f, 4.0f));
    }
    else {
        buttonShape.setPosition(buttonPosition);
        shadowShape.setPosition(buttonPosition + sf::Vector2f(3.0f, 3.0f));
    }

    buttonShape.setFillColor(fill);
    buttonShape.setOutlineColor(border);
    buttonShape.setOutlineThickness(outlineThickness);
}

void Button::updateAll(sf::Vector2i mousePosition, sf::RenderWindow* window)
{
    bool anyHovered = false;
    for (Button* button : allButtons) {
        const bool hovered = button->isClicked(mousePosition);
        if (hovered != button->isHovered) {
            button->isHovered = hovered;
            button->applyVisualState();
        }
        if (hovered) {
            anyHovered = true;
        }
    }

    if (!window) {
        return;
    }

#if defined(SFML_VERSION_MAJOR) && (SFML_VERSION_MAJOR > 2 || (SFML_VERSION_MAJOR == 2 && SFML_VERSION_MINOR >= 5))
    if (!cursorsLoaded) {
        cursorsLoaded = handCursor.loadFromSystem(sf::Cursor::Hand)
            && arrowCursor.loadFromSystem(sf::Cursor::Arrow);
    }
    if (cursorsLoaded) {
        window->setMouseCursor(anyHovered ? handCursor : arrowCursor);
    }
#else
    (void)anyHovered;
#endif
}

void Button::resetCursor(sf::RenderWindow* window)
{
    if (!window) {
        return;
    }

#if defined(SFML_VERSION_MAJOR) && (SFML_VERSION_MAJOR > 2 || (SFML_VERSION_MAJOR == 2 && SFML_VERSION_MINOR >= 5))
    if (!cursorsLoaded) {
        cursorsLoaded = handCursor.loadFromSystem(sf::Cursor::Hand)
            && arrowCursor.loadFromSystem(sf::Cursor::Arrow);
    }
    if (cursorsLoaded) {
        window->setMouseCursor(arrowCursor);
    }
#endif
}

bool Button::click(sf::Vector2i mousePosition)
{
    if (isClicked(mousePosition)) {
        setIsBeingClicked(true);
        return true;
    }
    return false;
}

bool Button::unclick()
{
    if (!isBeingClicked) {
        return false;
    }
    setIsBeingClicked(false);
    return true;
}

bool Button::unclickAll()
{
    const bool wasBeingClicked = isBeingClicked;
    for (Button* button : allButtons) {
        button->setIsBeingClicked(false);
    }
    return wasBeingClicked;
}

bool Button::isClicked(sf::Vector2i mousePosition) const
{
    return buttonShape.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosition));
}

void Button::setPosition(sf::Vector2f position)
{
    buttonPosition = position;
    setTextPosition();
    applyVisualState();
}

sf::Vector2f Button::getPosition() const
{
    return buttonPosition;
}

void Button::setSize(sf::Vector2f size)
{
    buttonSize = size;
    rebuildShapes();
    setTextPosition();
}

void Button::setSizeToText()
{
    const sf::FloatRect textBounds = buttonText.getLocalBounds();
    setSize(sf::Vector2f(textBounds.width * 1.35f, textBounds.height * 1.55f));
}

sf::Vector2f Button::getSize() const
{
    return buttonSize;
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
    customBaseColor = color;
    hasCustomBaseColor = color.a != 0;
    applyVisualState();
}

void Button::setOutlineColor(sf::Color color)
{
    customBorderColor = color;
    hasCustomBorderColor = true;
    applyVisualState();
}

void Button::setOutlineThickness(float thickness)
{
    outlineThickness = thickness;
    applyVisualState();
}

void Button::setIsBeingClicked(bool boolean)
{
    isBeingClicked = boolean;
    applyVisualState();
}

void Button::draw(sf::RenderTexture& window)
{
    window.draw(shadowShape);
    window.draw(buttonShape);
    window.draw(buttonText);
}

void Button::draw(sf::RenderWindow& window)
{
    const bool hovered = isClicked(sf::Mouse::getPosition(window));
    if (hovered != isHovered) {
        isHovered = hovered;
        applyVisualState();
    }

    window.draw(shadowShape);
    window.draw(buttonShape);
    window.draw(buttonText);
}

void Button::setTextPosition()
{
    const sf::FloatRect textBounds = buttonText.getLocalBounds();
    const float xPosition = buttonPosition.x + buttonSize.x / 2.0f - (textBounds.left + textBounds.width / 2.0f);
    const float yPosition = buttonPosition.y + buttonSize.y / 2.0f - (textBounds.top + textBounds.height / 2.0f) - 1.0f;
    buttonText.setPosition(xPosition, yPosition);
}
