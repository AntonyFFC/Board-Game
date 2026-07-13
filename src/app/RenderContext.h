#pragma once

#include "DisplayConfig.h"
#include "InputService.h"
#include "WindowManager.h"

struct RenderContext {
    WindowManager& windowManager;
    InputService& input;
    const DisplayConfig& display;

    sf::RenderWindow& window() { return windowManager.window(); }
    const sf::RenderWindow& window() const { return windowManager.window(); }
    sf::FloatRect logicalBounds() const { return windowManager.logicalBounds(); }
    sf::Vector2f mapPixelToLogical(sf::Vector2i pixel) const {
        return windowManager.mapPixelToLogical(pixel);
    }
};
