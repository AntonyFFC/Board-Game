#include "Layout.h"

#include <algorithm>
#include <cmath>

namespace Layout {

namespace {
    sf::Vector2f anchorPoint(Anchor anchor, const sf::FloatRect& parent)
    {
        switch (anchor) {
        case Anchor::TopLeft: return { parent.left, parent.top };
        case Anchor::TopCenter: return { parent.left + parent.width * 0.5f, parent.top };
        case Anchor::TopRight: return { parent.left + parent.width, parent.top };
        case Anchor::CenterLeft: return { parent.left, parent.top + parent.height * 0.5f };
        case Anchor::Center: return { parent.left + parent.width * 0.5f, parent.top + parent.height * 0.5f };
        case Anchor::CenterRight: return { parent.left + parent.width, parent.top + parent.height * 0.5f };
        case Anchor::BottomLeft: return { parent.left, parent.top + parent.height };
        case Anchor::BottomCenter: return { parent.left + parent.width * 0.5f, parent.top + parent.height };
        case Anchor::BottomRight: return { parent.left + parent.width, parent.top + parent.height };
        case Anchor::Stretch: return { parent.left, parent.top };
        }
        return {};
    }

    sf::Vector2f sizeOffset(Anchor anchor, sf::Vector2f size)
    {
        switch (anchor) {
        case Anchor::TopLeft:
        case Anchor::Stretch:
            return { 0.f, 0.f };
        case Anchor::TopCenter:
            return { -size.x * 0.5f, 0.f };
        case Anchor::TopRight:
            return { -size.x, 0.f };
        case Anchor::CenterLeft:
            return { 0.f, -size.y * 0.5f };
        case Anchor::Center:
            return { -size.x * 0.5f, -size.y * 0.5f };
        case Anchor::CenterRight:
            return { -size.x, -size.y * 0.5f };
        case Anchor::BottomLeft:
            return { 0.f, -size.y };
        case Anchor::BottomCenter:
            return { -size.x * 0.5f, -size.y };
        case Anchor::BottomRight:
            return { -size.x, -size.y };
        }
        return {};
    }
}

LayoutRect fromNormalized(const sf::FloatRect& parent,
    float nx, float ny, float nw, float nh)
{
    return {
        parent.left + parent.width * nx,
        parent.top + parent.height * ny,
        parent.width * nw,
        parent.height * nh
    };
}

LayoutRect inset(const sf::FloatRect& parent, float margin)
{
    return inset(parent, margin, margin, margin, margin);
}

LayoutRect inset(const sf::FloatRect& parent,
    float left, float top, float right, float bottom)
{
    return {
        parent.left + left,
        parent.top + top,
        std::max(0.f, parent.width - left - right),
        std::max(0.f, parent.height - top - bottom)
    };
}

LayoutRect resolve(Anchor anchor, const sf::FloatRect& parent,
    sf::Vector2f size, sf::Vector2f offset)
{
    if (anchor == Anchor::Stretch) {
        return { parent.left + offset.x, parent.top + offset.y,
            parent.width, parent.height };
    }

    const sf::Vector2f point = anchorPoint(anchor, parent);
    const sf::Vector2f anchorOffset = sizeOffset(anchor, size);
    return {
        point.x + anchorOffset.x + offset.x,
        point.y + anchorOffset.y + offset.y,
        size.x,
        size.y
    };
}

LayoutRect playArea(const sf::FloatRect& root,
    float topBarHeight, float bottomBarHeight, float sideMargin)
{
    return {
        root.left + sideMargin,
        root.top + topBarHeight,
        std::max(0.f, root.width - sideMargin * 2.f),
        std::max(0.f, root.height - topBarHeight - bottomBarHeight)
    };
}

LayoutRect uiBar(const sf::FloatRect& root, bool top, float height)
{
    if (top) {
        return { root.left, root.top, root.width, height };
    }
    return { root.left, root.top + root.height - height, root.width, height };
}

std::vector<LayoutRect> verticalStack(const sf::FloatRect& parent,
    size_t count, float itemHeight, float spacing,
    Anchor horizontalAnchor)
{
    (void)horizontalAnchor;
    std::vector<LayoutRect> slots;
    if (count == 0) {
        return slots;
    }

    const float totalHeight = static_cast<float>(count) * itemHeight
        + static_cast<float>(count - 1) * spacing;
    float y = parent.top + (parent.height - totalHeight) * 0.5f;

    const float itemWidth = std::min(320.f, parent.width * 0.28f);
    const float x = parent.left + (parent.width - itemWidth) * 0.5f;
    for (size_t i = 0; i < count; ++i) {
        slots.push_back({ x, y, itemWidth, itemHeight });
        y += itemHeight + spacing;
    }
    return slots;
}

void scaleSpriteToCover(sf::Sprite& sprite, const sf::FloatRect& area)
{
    const sf::FloatRect bounds = sprite.getLocalBounds();
    if (bounds.width <= 0.f || bounds.height <= 0.f) {
        return;
    }
    const float scaleX = area.width / bounds.width;
    const float scaleY = area.height / bounds.height;
    const float scale = std::max(scaleX, scaleY);
    sprite.setScale(scale, scale);
    sprite.setPosition(area.left, area.top);
}

void scaleSpriteToFit(sf::Sprite& sprite, const sf::FloatRect& area)
{
    const sf::FloatRect bounds = sprite.getLocalBounds();
    if (bounds.width <= 0.f || bounds.height <= 0.f) {
        return;
    }
    const float scaleX = area.width / bounds.width;
    const float scaleY = area.height / bounds.height;
    const float scale = std::min(scaleX, scaleY);
    sprite.setScale(scale, scale);
    const sf::FloatRect scaled = sprite.getGlobalBounds();
    sprite.setPosition(
        area.left + (area.width - scaled.width) * 0.5f,
        area.top + (area.height - scaled.height) * 0.5f);
}

} // namespace Layout
