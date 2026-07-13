#pragma once

#include <memory>

class Screen;

class ScreenNavigator {
public:
    virtual ~ScreenNavigator() = default;

    virtual void replaceScreen(std::unique_ptr<Screen> screen) = 0;
    virtual void pushScreen(std::unique_ptr<Screen> screen) = 0;
    virtual void popScreen() = 0;
    virtual void quitApplication() = 0;
};
