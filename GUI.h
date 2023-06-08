#pragma once

#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Board.h"
#include "Hex.h"
#include "Pawn.h"
#include <tuple>
#include <cmath>
#include "SelectionManager.h"

class Gui {
public:
    Gui(const int screenWidth = 1300, const int screenHeight = 1000);
    ~Gui();

    void start();
private:
    int screenWidth;
    int screenHeight;
    sf::RenderWindow* window;
    Board* grid;
};
