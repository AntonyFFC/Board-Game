#pragma once

#include "DisplayConfig.h"
#include <SFML/Graphics.hpp>

class WindowManager {
public:
    explicit WindowManager(const DisplayConfig& config);

    sf::RenderWindow& window();
    const sf::RenderWindow& window() const;
    const DisplayConfig& config() const;

    sf::FloatRect logicalBounds() const;
    float uniformScale() const;

    void applyView();
    void handleResize(unsigned width, unsigned height);
    void toggleFullscreen();

    sf::Vector2f mapPixelToLogical(sf::Vector2i pixel) const;

private:
    void createWindow();
    void updateLetterboxViewport();

    DisplayConfig config_;
    sf::RenderWindow window_;
    sf::View logicalView_;
    bool isFullscreen_ = false;
};
