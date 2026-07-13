#include "InputService.h"
#include "WindowManager.h"

InputService::InputService(WindowManager& windowManager)
    : windowManager_(windowManager)
{
    refresh();
}

void InputService::refresh()
{
    pixelMouse_ = sf::Mouse::getPosition(windowManager_.window());
    logicalMouse_ = windowManager_.mapPixelToLogical(pixelMouse_);
}

sf::Vector2f InputService::getLogicalMouse() const
{
    return logicalMouse_;
}

sf::Vector2i InputService::getPixelMouse() const
{
    return pixelMouse_;
}
