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
#include "SpriteUtils.h"
#include "Button.h"

class Gui {
public:
    Gui(sf::RenderWindow* window, Board* grid, Pawns* pawns);
    ~Gui();

    void rebuildLayout(const sf::FloatRect& root);
    void handleEvent(const sf::Event& event, sf::Vector2f logicalMouse);
    void update(float dt, sf::Vector2f logicalMouse, sf::RenderWindow* cursorWindow);
    void draw(sf::RenderTarget& target, sf::Vector2f logicalMouse);

private:
    sf::RenderWindow* window;
    Board* grid;
    Pawns* pawns_;
    Button endTurnButton;
    bool isShiftKeyPressed = false;
    sf::Sprite backgroundSprite;
    sf::Texture backgroundTexture;
    sf::FloatRect root_;
    sf::FloatRect playArea_;
    sf::FloatRect hudBar_;
    sf::Clock animationRedrawClock_;
    const float redrawInterval_ = 1.0f / 20.0f;
};
