#include "TextDamage.h"
#include "TableUtils.h"

TextDamage::TextDamage(const std::string& text, sf::Vector2f position, sf::Color colour, float duration)
	: lifetime(duration), elapsed(0.f), isSeen(true), color(colour)
{
    floatText = initializeText(text, &globalFont2, 30, colour);
    floatText.setPosition(position);
}

void TextDamage::update(float dt)
{
    if (!isSeen) return;

    elapsed += dt;

    floatText.move(0.0f, -30.0f * dt);
    float alpha = 255.f * (1.f - (elapsed / lifetime));
    if (alpha < 0.f) alpha = 0.f;

    sf::Color color = floatText.getFillColor();
    color.a = static_cast<sf::Uint8>(alpha);
    floatText.setFillColor(color);

    if (elapsed >= lifetime) {
        isSeen = false;
    }

}

void TextDamage::draw(sf::RenderTarget& target) const
{
    if (isSeen)
        target.draw(floatText);
}

bool TextDamage::isStillSeen() const
{
    return isSeen;
}