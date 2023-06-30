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
#include "Pawns.h"

class Gui {
public:
    Gui(const int screenWidth = 1300, const int screenHeight = 1000);
    ~Gui();

    void start();
    void keyPressed(const sf::Event& event);
private:
    int screenWidth;
    int screenHeight;
    sf::RenderWindow* window;
    Board* grid;
    Pawns* pawns;
    Pawn* p1;
    Pawn* p2;
    Equipment* e1;
    Equipment* e2;
    Equipment* e3;
    Equipment* e4;
    bool isShiftKeyPressed = false;
};