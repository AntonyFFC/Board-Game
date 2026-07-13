#include "app/Application.h"
#include "app/DisplayConfig.h"
#include "screens/MenuScreen.h"

#include <iostream>
#include <memory>

int main()
{
    std::cout << "Game is starting up..." << std::endl;

    DisplayConfig config;
    config.designWidth = 1920;
    config.designHeight = 1080;
    config.windowWidth = 1600;
    config.windowHeight = 900;
    config.startFullscreen = false;
    config.vsync = true;

    Application app(config);
    app.run(std::make_unique<MenuScreen>(app));
    return 0;
}
