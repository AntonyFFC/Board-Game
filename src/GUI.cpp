#include "GUI.h"

#include "ui/Layout.h"

Gui::Gui(sf::RenderWindow* window, Board* grid, Pawns* pawns)
	: window(window),
    grid(grid),
    pawns_(pawns),
    endTurnButton({}, { 200.f, 50.f }, "end turn")
{
    initializeFont();
    backgroundSprite = loadBackgroundSprite(&backgroundTexture, "board");
}

Gui::~Gui() = default;

void Gui::rebuildLayout(const sf::FloatRect& root)
{
    root_ = root;
    playArea_ = Layout::playArea(root, 80.f, 90.f, 40.f).toFloatRect();
    hudBar_ = Layout::uiBar(root, false, 90.f).toFloatRect();

    Layout::scaleSpriteToCover(backgroundSprite, root);
    grid->layoutInArea(playArea_);

    const float barCenterY = hudBar_.top + hudBar_.height * 0.5f;
    endTurnButton.setPosition({
        root.left + root.width * 0.5f + 150.f,
        barCenterY - 25.f });
}

void Gui::handleEvent(const sf::Event& event, sf::Vector2f logicalMouse)
{
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::LShift) {
        isShiftKeyPressed = true;
        pawns_->handleShiftOn();
    }
    else if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::LShift) {
        isShiftKeyPressed = false;
        pawns_->handleShiftOff();
    }
    else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
		if (!endTurnButton.click(logicalMouse)) {
            pawns_->handleClick(logicalMouse);
        }
    }
    else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right) {
        pawns_->handleClickRight(logicalMouse);
    }
    else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
        if (endTurnButton.unclick()) {
            pawns_->endTurn();
        }
        else {
			pawns_->handleClickRelease(logicalMouse);
        }
    }
}

void Gui::update(float dt, sf::Vector2f logicalMouse, sf::RenderWindow* cursorWindow)
{
    pawns_->processDeferredWork();
    pawns_->updateAnimations(dt);

    const bool buttonHovered = Button::updateAll(logicalMouse, nullptr);
    const bool boardHovered = pawns_->updateHover(logicalMouse);
    Button::applyCursor(cursorWindow, buttonHovered || boardHovered);

    if (pawns_->needsContinuousRedraw()) {
        pawns_->finalizePendingMoveIfReady();
        animationRedrawClock_.restart();
    }
    else if (animationRedrawClock_.getElapsedTime().asSeconds() >= redrawInterval_
        && pawns_->hasActiveAnimations()) {
        animationRedrawClock_.restart();
    }
}

void Gui::draw(sf::RenderTarget& target, sf::Vector2f logicalMouse)
{
    (void)logicalMouse;
    target.draw(backgroundSprite);
    grid->drawBoard(target);
    pawns_->draw(isShiftKeyPressed);
	endTurnButton.draw(dynamic_cast<sf::RenderWindow&>(target));
}
