#include "screens/ShopScreen.h"

#include "screens/WarriorPrepScreen.h"

ShopScreen::ShopScreen(ScreenNavigator& navigator)
    : navigator_(navigator)
{
}

void ShopScreen::onEnter(RenderContext& context)
{
    (void)context;
    shop_.prepare();
}

void ShopScreen::onExit(RenderContext& context)
{
    (void)context;
}

void ShopScreen::onResize(RenderContext& context)
{
    shop_.rebuildLayout(context.logicalBounds());
}

void ShopScreen::handleEvent(const sf::Event& event, RenderContext& context)
{
    if (event.type == sf::Event::MouseButtonPressed
        || event.type == sf::Event::MouseButtonReleased) {
        shop_.handleEvent(event, context.mapPixelToLogical(
            { event.mouseButton.x, event.mouseButton.y }));
    }
    else if (event.type == sf::Event::KeyPressed) {
        shop_.handleEvent(event, context.input.getLogicalMouse());
    }
}

void ShopScreen::update(float dt, RenderContext& context)
{
    (void)dt;
    shop_.update(context.input.getLogicalMouse(), &context.window());

    if (shop_.wantsStartGame()) {
        navigator_.replaceScreen(std::make_unique<WarriorPrepScreen>(
            navigator_, shop_.collectLaunchData()));
    }
}

void ShopScreen::draw(sf::RenderTarget& target, RenderContext& context)
{
    shop_.draw(target, context.input.getLogicalMouse());
}
