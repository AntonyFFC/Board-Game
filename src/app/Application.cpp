#include "Application.h"

Application::Application(DisplayConfig config)
    : config_(config)
    , windowManager_(config_)
    , input_(windowManager_)
    , renderContext_{ windowManager_, input_, config_ }
{
}

RenderContext& Application::renderContext()
{
    return renderContext_;
}

void Application::activateScreen(Screen& screen, bool entering)
{
    if (entering) {
        screen.onEnter(renderContext_);
        screen.onResize(renderContext_);
    }
    else {
        screen.onExit(renderContext_);
    }
}

void Application::replaceScreen(std::unique_ptr<Screen> screen)
{
    if (!screenStack_.empty()) {
        activateScreen(*screenStack_.back(), false);
        screenStack_.clear();
    }
    screenStack_.push_back(std::move(screen));
    activateScreen(*screenStack_.back(), true);
}

void Application::pushScreen(std::unique_ptr<Screen> screen)
{
    screenStack_.push_back(std::move(screen));
    activateScreen(*screenStack_.back(), true);
}

void Application::popScreen()
{
    if (screenStack_.empty()) {
        return;
    }
    activateScreen(*screenStack_.back(), false);
    screenStack_.pop_back();
    if (!screenStack_.empty()) {
        activateScreen(*screenStack_.back(), true);
    }
}

void Application::quitApplication()
{
    windowManager_.window().close();
}

void Application::run(std::unique_ptr<Screen> initialScreen)
{
    replaceScreen(std::move(initialScreen));

    sf::Clock frameClock;
    while (windowManager_.window().isOpen()) {
        const float dt = frameClock.restart().asSeconds();

        sf::Event event;
        while (windowManager_.window().pollEvent(event)) {
            processEvent(event);
        }

        if (!windowManager_.window().isOpen()) {
            break;
        }

        if (!screenStack_.empty()) {
            input_.refresh();
            screenStack_.back()->update(dt, renderContext_);
            drawFrame();
        }
    }

    while (!screenStack_.empty()) {
        activateScreen(*screenStack_.back(), false);
        screenStack_.pop_back();
    }
}

void Application::processEvent(const sf::Event& event)
{
    if (event.type == sf::Event::Closed) {
        windowManager_.window().close();
        return;
    }

    if (event.type == sf::Event::Resized) {
        windowManager_.handleResize(event.size.width, event.size.height);
        if (!screenStack_.empty()) {
            screenStack_.back()->onResize(renderContext_);
        }
    }

    if (event.type == sf::Event::KeyPressed
        && event.key.code == sf::Keyboard::F11) {
        windowManager_.toggleFullscreen();
        if (!screenStack_.empty()) {
            screenStack_.back()->onResize(renderContext_);
        }
        return;
    }

    if (!screenStack_.empty()) {
        screenStack_.back()->handleEvent(event, renderContext_);
    }
}

void Application::drawFrame()
{
    windowManager_.applyView();
    windowManager_.window().clear(sf::Color(71, 31, 16));

    if (!screenStack_.empty()) {
        screenStack_.back()->draw(windowManager_.window(), renderContext_);
    }

    windowManager_.window().display();
}
