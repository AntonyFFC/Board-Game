#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

enum class Anchor {
    TopLeft,
    TopCenter,
    TopRight,
    CenterLeft,
    Center,
    CenterRight,
    BottomLeft,
    BottomCenter,
    BottomRight,
    Stretch
};

struct LayoutRect {
    float x = 0.f;
    float y = 0.f;
    float width = 0.f;
    float height = 0.f;

    sf::FloatRect toFloatRect() const {
        return sf::FloatRect(x, y, width, height);
    }

    sf::Vector2f center() const {
        return sf::Vector2f(x + width * 0.5f, y + height * 0.5f);
    }
};

namespace Layout {

LayoutRect fromNormalized(const sf::FloatRect& parent,
    float nx, float ny, float nw, float nh);

LayoutRect inset(const sf::FloatRect& parent, float margin);
LayoutRect inset(const sf::FloatRect& parent,
    float left, float top, float right, float bottom);

LayoutRect resolve(Anchor anchor, const sf::FloatRect& parent,
    sf::Vector2f size, sf::Vector2f offset = {});

LayoutRect playArea(const sf::FloatRect& root,
    float topBarHeight = 80.f, float bottomBarHeight = 90.f,
    float sideMargin = 40.f);

LayoutRect uiBar(const sf::FloatRect& root, bool top, float height);

std::vector<LayoutRect> verticalStack(const sf::FloatRect& parent,
    size_t count, float itemHeight, float spacing,
    Anchor horizontalAnchor = Anchor::Center);

void scaleSpriteToCover(sf::Sprite& sprite, const sf::FloatRect& area);
void scaleSpriteToFit(sf::Sprite& sprite, const sf::FloatRect& area);

} // namespace Layout
