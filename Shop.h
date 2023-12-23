#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include <string>
#include "Equipment.h"
#include "Pawn.h"
#include "Globals.h"
#include "Card.h"
#include "EquipmentManager.h"
#include "PawnsManager.h"
#include <algorithm>
#include "Button.h"
#include <iostream>
#include "IconWall.h"
#include "ShopPawns.h"
#include "ShopStorage.h"
#include "GUI.h"
#include "ShopCards.h"

class Shop {
public:
    friend class IconWall;

    Shop(sf::RenderWindow* window);
    ~Shop();

    void start();

    // Proceed to the next turn and update the shop
    void nextTurn();

private:
    sf::RenderWindow* window;
    EquipmentCard* lastItem;
    Gui* interface1;
    ShopCards* shopCards;

    int currentRound;
    int currentPlayerIndex;
    int remainingGold;
    int fontSize;
    sf::Sprite backgroundSprite;
    sf::Texture backgroundTexture;
    sf::Text titleText;
    sf::Text blueTurnText;
    sf::Text redTurnText;
    sf::Text goldText;
    sf::Text roundText;
    IconWall wallIcon;
    Button changeButton;
    Button nextPlayer;
    ShopPawns shopPawns[2];
    ShopStorage shopStorage[2];

    // Player buys a card or item from the shop
    bool buy(int cardNum);
    bool buyWall();
    void addCard(int cardNum);
    void reduceMoney(int price);
    void addLastItemToStorage();

    // Display the available cards and items in the shop
    void displayShop();
    void drawChangeButton();
    void drawNextPlayerButton();
    void drawTurn();

    // Private helper functions for managing the shop
    void resetShop();

    void keyPressed(const sf::Event& event);
    void whatClicked(sf::Vector2i mousePosition);
    void whatOffClicked(sf::Vector2i mousePosition);
    void updateGoldText();

    void unClickAll();

    void startGame();

};