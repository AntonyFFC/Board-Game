#pragma once

#include "app/Screen.h"
#include "app/RenderContext.h"
#include "app/ScreenNavigator.h"
#include "Button.h"
#include "ui/Layout.h"

#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include <vector>

class MenuScreen : public Screen {
public:
    explicit MenuScreen(ScreenNavigator& navigator);

    void onEnter(RenderContext& context) override;
    void onExit(RenderContext& context) override;
    void onResize(RenderContext& context) override;
    void handleEvent(const sf::Event& event, RenderContext& context) override;
    void update(float dt, RenderContext& context) override;
    void draw(sf::RenderTarget& target, RenderContext& context) override;

private:
    void rebuildLayout(const sf::FloatRect& root);
    void callSelected(int selected, RenderContext& context);
    int getSelectedItem() const;
    void initializeAssets();

    ScreenNavigator& navigator_;
    std::vector<std::string> buttonLabels_;
    std::vector<std::unique_ptr<Button>> buttons_;
    int selectedIndex_ = 0;

    LayoutRect backgroundLayer_;
    LayoutRect uiLayer_;
    LayoutRect titlePanel_;
    LayoutRect menuColumn_;
    sf::Vector2f titlePosition_;

    sf::Sprite backgroundSprite_;
    sf::Texture backgroundTexture_;
    bool assetsReady_ = false;
};
