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
#include <random> 
#include "Button.h"
#include <iostream>
#include "IconWall.h"
#include "ShopPawns.h"
#include "ShopStorage.h"

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
    std::vector<WarriorCard*> warriorsCards;
    std::vector<EquipmentCard*> itemsCards;
    std::vector<Equipment*> equipmentList;
    std::vector<Pawn*> pawnsList;
    std::vector<Pawn*> playerWarriors[2];
    std::vector<Equipment*> playerItems[2];
    int numberOfWalls[2] = { 0, 0 };
    EquipmentCard* lastItem;

    int currentRound;
    int currentPlayerIndex;
    bool currentPage;
    int remainingGold;
    int fontSize;
    sf::Sprite backgroundSprite;
    sf::Texture backgroundTexture;
    sf::Text titleText;
    sf::Text blueTurnText;
    sf::Text redTurnText;
    sf::Text goldText;
    IconWall wallIcon;
    Button changeButton;
    ShopPawns shopPawns[2];
    ShopStorage shopStorage[2];

    // Player buys a card or item from the shop
    bool buy(int cardNum);
    bool buyWall();
    int getPrice(int cardNum);
    void addCard(int cardNum);
    void removeShopCard(int cardNum);
    void reduceMoney(int price);

    // Display the available cards and items in the shop
    void displayShop();
    void drawCards();
    void drawChangeButton();
    void drawTurn();

    // Private helper functions for managing the shop
    void resetShop();

    void keyPressed(const sf::Event& event);
    int whichCardClicked(sf::Vector2i mousePosition); // returns -1 id none

    void updateDecks();
    void initializeCards(std::vector<Equipment*> availableItems, std::vector<Pawn*> availableWarriors);

    void flipPage();
    void updateGoldText();

    void unClickAll();

    // Add more private members and methods as needed

};