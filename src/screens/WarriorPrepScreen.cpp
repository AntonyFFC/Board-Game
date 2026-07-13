#include "screens/WarriorPrepScreen.h"

#include "screens/GameScreen.h"
#include "ui/Layout.h"

WarriorPrepScreen::WarriorPrepScreen(ScreenNavigator& navigator, GameLaunchData launchData)
    : navigator_(navigator)
    , launchData_(std::move(launchData))
{
}

void WarriorPrepScreen::onEnter(RenderContext& context)
{
    board_ = std::make_unique<Board>(13, 19, 0.8f);
    pawns_ = std::make_unique<Pawns>(board_.get(), &context.window());
    prep_ = std::make_unique<WarriorPrep>(board_.get(), pawns_.get());

    prep_->addPawns(launchData_.pawns[0], 0);
    prep_->addPawns(launchData_.pawns[1], 1);
    pawns_->addWalls(launchData_.walls[0], 0);
    pawns_->addWalls(launchData_.walls[1], 1);
}

void WarriorPrepScreen::onExit(RenderContext& context)
{
    (void)context;
}

void WarriorPrepScreen::onResize(RenderContext& context)
{
    const sf::FloatRect root = context.logicalBounds();
    playArea_ = Layout::playArea(root, 120.f, 40.f, 40.f).toFloatRect();
    board_->layoutInArea(playArea_);
    prep_->rebuildLayout(root, playArea_);
}

void WarriorPrepScreen::handleEvent(const sf::Event& event, RenderContext& context)
{
    if (event.type == sf::Event::MouseButtonPressed) {
        const sf::Vector2f mouse = context.mapPixelToLogical(
            { event.mouseButton.x, event.mouseButton.y });
        prep_->handleEvent(event, mouse);
    }
}

void WarriorPrepScreen::update(float dt, RenderContext& context)
{
    (void)dt;
    (void)context;

    if (prep_->isFinished()) {
        navigator_.replaceScreen(std::make_unique<GameScreen>(
            navigator_,
            std::move(board_),
            std::move(pawns_),
            launchData_));
    }
}

void WarriorPrepScreen::draw(sf::RenderTarget& target, RenderContext& context)
{
    (void)context;
    prep_->draw(target);
}
