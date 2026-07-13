#include "WindowManager.h"

#include <algorithm>
#include <cmath>

WindowManager::WindowManager(const DisplayConfig& config)
    : config_(config)
{
    logicalView_.reset(sf::FloatRect(0.f, 0.f,
        static_cast<float>(config_.designWidth),
        static_cast<float>(config_.designHeight)));
    createWindow();
    applyView();
}

sf::RenderWindow& WindowManager::window()
{
    return window_;
}

const sf::RenderWindow& WindowManager::window() const
{
    return window_;
}

const DisplayConfig& WindowManager::config() const
{
    return config_;
}

sf::FloatRect WindowManager::logicalBounds() const
{
    return sf::FloatRect(0.f, 0.f,
        static_cast<float>(config_.designWidth),
        static_cast<float>(config_.designHeight));
}

float WindowManager::uniformScale() const
{
    const sf::Vector2u size = window_.getSize();
    if (size.x == 0 || size.y == 0) {
        return 1.f;
    }
    const float scaleX = size.x / static_cast<float>(config_.designWidth);
    const float scaleY = size.y / static_cast<float>(config_.designHeight);
    return std::min(scaleX, scaleY);
}

void WindowManager::createWindow()
{
    const sf::Uint32 style = config_.startFullscreen
        ? sf::Style::Fullscreen
        : sf::Style::Default;

    if (config_.startFullscreen) {
        const sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
        window_.create(desktop, "Skirmish", style);
        isFullscreen_ = true;
    }
    else {
        window_.create(sf::VideoMode(config_.windowWidth, config_.windowHeight),
            "Skirmish", style);
        isFullscreen_ = false;
    }

    window_.setVerticalSyncEnabled(config_.vsync);
}

void WindowManager::updateLetterboxViewport()
{
    const sf::Vector2u size = window_.getSize();
    if (size.x == 0 || size.y == 0) {
        return;
    }

    const float windowAspect = size.x / static_cast<float>(size.y);
    const float designAspect = static_cast<float>(config_.designWidth)
        / static_cast<float>(config_.designHeight);

    sf::FloatRect viewport(0.f, 0.f, 1.f, 1.f);
    if (windowAspect > designAspect) {
        const float viewWidth = designAspect / windowAspect;
        viewport.left = (1.f - viewWidth) * 0.5f;
        viewport.width = viewWidth;
    }
    else {
        const float viewHeight = windowAspect / designAspect;
        viewport.top = (1.f - viewHeight) * 0.5f;
        viewport.height = viewHeight;
    }

    logicalView_.setViewport(viewport);
}

void WindowManager::applyView()
{
    updateLetterboxViewport();
    window_.setView(logicalView_);
}

void WindowManager::handleResize(unsigned width, unsigned height)
{
    (void)width;
    (void)height;
    applyView();
}

sf::Vector2f WindowManager::mapPixelToLogical(sf::Vector2i pixel) const
{
    const sf::Vector2u size = window_.getSize();
    if (size.x == 0 || size.y == 0) {
        return {};
    }

    const float windowAspect = size.x / static_cast<float>(size.y);
    const float designAspect = static_cast<float>(config_.designWidth)
        / static_cast<float>(config_.designHeight);

    sf::FloatRect viewport(0.f, 0.f, 1.f, 1.f);
    if (windowAspect > designAspect) {
        const float viewWidth = designAspect / windowAspect;
        viewport.left = (1.f - viewWidth) * 0.5f;
        viewport.width = viewWidth;
    }
    else {
        const float viewHeight = windowAspect / designAspect;
        viewport.top = (1.f - viewHeight) * 0.5f;
        viewport.height = viewHeight;
    }

    sf::View view = logicalView_;
    view.setViewport(viewport);
    return window_.mapPixelToCoords(pixel, view);
}

void WindowManager::toggleFullscreen()
{
    isFullscreen_ = !isFullscreen_;
    const sf::Vector2u previousSize = window_.getSize();

    if (isFullscreen_) {
        window_.create(sf::VideoMode::getDesktopMode(), "Skirmish", sf::Style::Fullscreen);
    }
    else {
        window_.create(sf::VideoMode(config_.windowWidth, config_.windowHeight),
            "Skirmish", sf::Style::Default);
    }

    window_.setVerticalSyncEnabled(config_.vsync);
    if (previousSize.x > 0 && previousSize.y > 0) {
        applyView();
    }
}
