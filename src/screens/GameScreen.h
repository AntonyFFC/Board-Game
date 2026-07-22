#pragma once

#include "app/GameLaunchData.h"
#include "app/Screen.h"
#include "app/RenderContext.h"
#include "app/ScreenNavigator.h"
#include "Board.h"
#include "Button.h"
#include "GUI.h"
#include "Pawns.h"
#include "ui/Layout.h"

#include <memory>

class GameScreen : public Screen {
public:
    GameScreen(ScreenNavigator& navigator,
        std::unique_ptr<Board> board,
        std::unique_ptr<Pawns> pawns,
        const GameLaunchData& launchData);
    ~GameScreen() override = default;

    void onEnter(RenderContext& context) override;
    void onExit(RenderContext& context) override;
    void onResize(RenderContext& context) override;
    void handleEvent(const sf::Event& event, RenderContext& context) override;
    void update(float dt, RenderContext& context) override;
    void draw(sf::RenderTarget& target, RenderContext& context) override;

private:
    void rebuildPauseLayout(const sf::FloatRect& root);
    void handlePauseEvent(const sf::Event& event, RenderContext& context);
    void drawPauseOverlay(sf::RenderTarget& target);

    ScreenNavigator& navigator_;
    std::unique_ptr<Board> board_;
    std::unique_ptr<Pawns> pawns_;
    std::unique_ptr<Gui> gui_;

    bool paused_ = false;
    Button resumeButton_;
    Button quitButton_;
    sf::RectangleShape pauseDim_;
    sf::Text pauseTitle_;
};
