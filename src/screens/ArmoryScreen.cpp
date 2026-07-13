#include "screens/ArmoryScreen.h"

#include "screens/MenuScreen.h"

ArmoryScreen::ArmoryScreen(ScreenNavigator& navigator)
    : navigator_(navigator)
{
}

void ArmoryScreen::onEnter(RenderContext& context)
{
    (void)context;
    armory_.resetExit();
}

void ArmoryScreen::onExit(RenderContext& context)
{
    (void)context;
}

void ArmoryScreen::onResize(RenderContext& context)
{
    armory_.rebuildLayout(context.logicalBounds());
}

void ArmoryScreen::handleEvent(const sf::Event& event, RenderContext& context)
{
    if (event.type == sf::Event::MouseButtonPressed) {
        armory_.handleEvent(event, context.mapPixelToLogical(
            { event.mouseButton.x, event.mouseButton.y }));
    }
    else if (event.type == sf::Event::KeyPressed) {
        armory_.handleEvent(event, context.input.getLogicalMouse());
    }
}

void ArmoryScreen::update(float dt, RenderContext& context)
{
    (void)dt;
    armory_.update(context.input.getLogicalMouse(), &context.window());

    if (armory_.wantsExit()) {
        navigator_.popScreen();
    }
}

void ArmoryScreen::draw(sf::RenderTarget& target, RenderContext& context)
{
    (void)context;
    armory_.draw(target);
}
