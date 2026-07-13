#pragma once

#include <SFML/Graphics.hpp>

class WindowManager;

class InputService {
public:
    explicit InputService(WindowManager& windowManager);

    void refresh();
    sf::Vector2f getLogicalMouse() const;
    sf::Vector2i getPixelMouse() const;

private:
    WindowManager& windowManager_;
    sf::Vector2f logicalMouse_;
    sf::Vector2i pixelMouse_;
};
