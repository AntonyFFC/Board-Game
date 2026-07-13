#include "screens/GameScreen.h"

#include "screens/MenuScreen.h"

GameScreen::GameScreen(ScreenNavigator& navigator,
    std::unique_ptr<Board> board,
    std::unique_ptr<Pawns> pawns,
    const GameLaunchData& launchData)
    : navigator_(navigator)
    , board_(std::move(board))
    , pawns_(std::move(pawns))
{
    (void)launchData;
}

void GameScreen::onEnter(RenderContext& context)
{
    gui_ = std::make_unique<Gui>(&context.window(), board_.get(), pawns_.get());
}

void GameScreen::onExit(RenderContext& context)
{
    (void)context;
    gui_.reset();
}

void GameScreen::onResize(RenderContext& context)
{
    if (gui_) {
        gui_->rebuildLayout(context.logicalBounds());
    }
}

void GameScreen::handleEvent(const sf::Event& event, RenderContext& context)
{
    if (!gui_) {
        return;
    }

    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
        navigator_.replaceScreen(std::make_unique<MenuScreen>(navigator_));
        return;
    }

    sf::Vector2f mouse = context.input.getLogicalMouse();
    if (event.type == sf::Event::MouseButtonPressed
        || event.type == sf::Event::MouseButtonReleased) {
        mouse = context.mapPixelToLogical({ event.mouseButton.x, event.mouseButton.y });
    }
    gui_->handleEvent(event, mouse);
}

void GameScreen::update(float dt, RenderContext& context)
{
    if (gui_) {
        gui_->update(dt, context.input.getLogicalMouse(), &context.window());
    }
}

void GameScreen::draw(sf::RenderTarget& target, RenderContext& context)
{
    if (gui_) {
        gui_->draw(target, context.input.getLogicalMouse());
    }
}
