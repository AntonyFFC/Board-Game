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
#include "WarriorPrep.h"
#include "Button.h"

class Gui {
public:
    Gui(sf::RenderWindow* window);
    ~Gui();

    void start();
    void keyPressed(const sf::Event& event);
    void addPawns(std::vector<Pawn*> pawns, int playerIndx);
	void addWalls(int numWalls, int playerIndx);
private:
    void display();

    sf::RenderWindow* window;
    Board* grid;
    Pawns* pawns;
	Button endTurnButton;
    WarriorPrep* warriorPrep;
    bool isShiftKeyPressed = false;
    sf::Sprite backgroundSprite;
    sf::Texture backgroundTexture;
};