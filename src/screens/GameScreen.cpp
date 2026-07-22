#include "screens/GameScreen.h"

#include "screens/MenuScreen.h"
#include "Globals.h"

GameScreen::GameScreen(ScreenNavigator& navigator,
    std::unique_ptr<Board> board,
    std::unique_ptr<Pawns> pawns,
    const GameLaunchData& launchData)
    : navigator_(navigator)
    , board_(std::move(board))
    , pawns_(std::move(pawns))
    , resumeButton_({}, { 280.f, 56.f }, "Resume")
    , quitButton_({}, { 280.f, 56.f }, "Quit to Menu")
{
    (void)launchData;
    initializeFont();
    pauseTitle_ = initializeText("Paused", &globalFont2, 48, sf::Color::White);
    pauseDim_.setFillColor(sf::Color(20, 12, 8, 160));
}

void GameScreen::onEnter(RenderContext& context)
{
    gui_ = std::make_unique<Gui>(&context.window(), board_.get(), pawns_.get());
    paused_ = false;
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
    rebuildPauseLayout(context.logicalBounds());
}

void GameScreen::rebuildPauseLayout(const sf::FloatRect& root)
{
    pauseDim_.setPosition(root.left, root.top);
    pauseDim_.setSize({ root.width, root.height });

    const sf::FloatRect titleBounds = pauseTitle_.getLocalBounds();
    pauseTitle_.setOrigin(titleBounds.width * 0.5f, 0.f);
    pauseTitle_.setPosition(root.left + root.width * 0.5f, root.top + root.height * 0.32f);

    const LayoutRect resumeSlot = Layout::resolve(Anchor::Center, root, { 280.f, 56.f }, { 0.f, 10.f });
    const LayoutRect quitSlot = Layout::resolve(Anchor::Center, root, { 280.f, 56.f }, { 0.f, 80.f });
    resumeButton_.setPosition({ resumeSlot.x, resumeSlot.y });
    quitButton_.setPosition({ quitSlot.x, quitSlot.y });
}

void GameScreen::handlePauseEvent(const sf::Event& event, RenderContext& context)
{
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
        paused_ = false;
        return;
    }

    if (event.type == sf::Event::MouseButtonPressed
        && event.mouseButton.button == sf::Mouse::Left) {
        const sf::Vector2f mouse = context.mapPixelToLogical(
            { event.mouseButton.x, event.mouseButton.y });
        if (resumeButton_.isClicked(mouse)) {
            resumeButton_.click(mouse);
        }
        else if (quitButton_.isClicked(mouse)) {
            quitButton_.click(mouse);
        }
    }
    else if (event.type == sf::Event::MouseButtonReleased
        && event.mouseButton.button == sf::Mouse::Left) {
        const sf::Vector2f mouse = context.mapPixelToLogical(
            { event.mouseButton.x, event.mouseButton.y });
        if (resumeButton_.unclick() && resumeButton_.isClicked(mouse)) {
            paused_ = false;
        }
        else if (quitButton_.unclick() && quitButton_.isClicked(mouse)) {
            navigator_.replaceScreen(std::make_unique<MenuScreen>(navigator_));
        }
        else {
            resumeButton_.unclick();
            quitButton_.unclick();
        }
    }
}

void GameScreen::handleEvent(const sf::Event& event, RenderContext& context)
{
    if (!gui_) {
        return;
    }

    if (paused_) {
        handlePauseEvent(event, context);
        return;
    }

    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
        paused_ = true;
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
    if (!gui_) {
        return;
    }

    if (paused_) {
        Button::updateAll(context.input.getLogicalMouse(), &context.window());
        return;
    }

    gui_->update(dt, context.input.getLogicalMouse(), &context.window());
}

void GameScreen::drawPauseOverlay(sf::RenderTarget& target)
{
    target.draw(pauseDim_);
    target.draw(pauseTitle_);
    resumeButton_.draw(dynamic_cast<sf::RenderWindow&>(target));
    quitButton_.draw(dynamic_cast<sf::RenderWindow&>(target));
}

void GameScreen::draw(sf::RenderTarget& target, RenderContext& context)
{
    if (gui_) {
        gui_->draw(target, context.input.getLogicalMouse());
    }
    if (paused_) {
        drawPauseOverlay(target);
    }
}
