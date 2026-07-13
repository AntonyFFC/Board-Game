#pragma once

#include "app/Screen.h"
#include "app/RenderContext.h"
#include "app/ScreenNavigator.h"
#include "Shop.h"

class ShopScreen : public Screen {
public:
    explicit ShopScreen(ScreenNavigator& navigator);

    void onEnter(RenderContext& context) override;
    void onExit(RenderContext& context) override;
    void onResize(RenderContext& context) override;
    void handleEvent(const sf::Event& event, RenderContext& context) override;
    void update(float dt, RenderContext& context) override;
    void draw(sf::RenderTarget& target, RenderContext& context) override;

private:
    ScreenNavigator& navigator_;
    Shop shop_;
};
