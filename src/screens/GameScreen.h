#pragma once

#include "app/GameLaunchData.h"
#include "app/Screen.h"
#include "app/RenderContext.h"
#include "app/ScreenNavigator.h"
#include "Board.h"
#include "GUI.h"
#include "Pawns.h"

#include <memory>

class GameScreen : public Screen {
public:
    GameScreen(ScreenNavigator& navigator,
        std::unique_ptr<Board> board,
        std::unique_ptr<Pawns> pawns,
        const GameLaunchData& launchData);

    void onEnter(RenderContext& context) override;
    void onExit(RenderContext& context) override;
    void onResize(RenderContext& context) override;
    void handleEvent(const sf::Event& event, RenderContext& context) override;
    void update(float dt, RenderContext& context) override;
    void draw(sf::RenderTarget& target, RenderContext& context) override;

private:
    ScreenNavigator& navigator_;
    std::unique_ptr<Board> board_;
    std::unique_ptr<Pawns> pawns_;
    std::unique_ptr<Gui> gui_;
};
