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

class Shop {
public:
    friend class IconWall;

    Shop(sf::RenderWindow* window);

    void start();

    // Initialize the shop with available cards and items
    void initializeShop();

    // Proceed to the next turn and update the shop
    void nextTurn();

private:
    sf::RenderWindow* window;
    std::vector<std::shared_ptr<Card>> shownCards;
    std::vector<std::shared_ptr<Card>> warriorsCards;
    std::vector<std::shared_ptr<Card>> itemsCards;
    std::vector<Equipment*> equipmentList;
    std::vector<Pawn*> pawnsList;
    std::vector<Pawn*> playerWarriors[2];
    std::vector<Equipment*> playerItems[2];
    int numberOfWalls[2] = { 0, 0 };

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

    // Player buys a card or item from the shop
    bool buy(int cardNum);
    bool buyWall();
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
    void assignCards();
    void updateGoldText();

    // Add more private members and methods as needed

};