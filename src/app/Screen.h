#pragma once

#include "RenderContext.h"
#include <SFML/Window/Event.hpp>

class Screen {
public:
    virtual ~Screen() = default;

    virtual void onEnter(RenderContext& context) = 0;
    virtual void onExit(RenderContext& context) = 0;
    virtual void onResize(RenderContext& context) = 0;
    virtual void handleEvent(const sf::Event& event, RenderContext& context) = 0;
    virtual void update(float dt, RenderContext& context) = 0;
    virtual void draw(sf::RenderTarget& target, RenderContext& context) = 0;
};
