#pragma once

#include "DisplayConfig.h"
#include "InputService.h"
#include "RenderContext.h"
#include "Screen.h"
#include "ScreenNavigator.h"
#include "WindowManager.h"

#include <SFML/System/Clock.hpp>
#include <memory>
#include <vector>

class Application : public ScreenNavigator {
public:
    explicit Application(DisplayConfig config = {});

    void run(std::unique_ptr<Screen> initialScreen);
    RenderContext& renderContext();

    void replaceScreen(std::unique_ptr<Screen> screen) override;
    void pushScreen(std::unique_ptr<Screen> screen) override;
    void popScreen() override;
    void quitApplication() override;

private:
    void processEvent(const sf::Event& event);
    void drawFrame();
    void activateScreen(Screen& screen, bool entering);

    DisplayConfig config_;
    WindowManager windowManager_;
    InputService input_;
    RenderContext renderContext_;
    std::vector<std::unique_ptr<Screen>> screenStack_;
    sf::Clock clock_;
};
