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

class Shop {
public:
    Shop(sf::RenderWindow* window);
    ~Shop();

    void start();

    // Initialize the shop with available cards and items
    void initializeShop();

    // Player buys a card or item from the shop
    bool buy(int player, int card);

    // Proceed to the next turn and update the shop
    void nextTurn();

private:
    sf::RenderWindow* window;
    std::vector<Card> shownCards;
    std::vector<Equipment*> equipmentList;
    std::vector<Pawn*> pawnsList;
    std::vector<Equipment*> availableItems;
    std::vector<Pawn*> availableWarriors;
    int currentRound;
    int currentPlayerIndex;
    int remainingGold;
    sf::Sprite backgroundSprite;
    sf::Texture backgroundTexture;
    sf::Text titleText;

    // Display the available cards and items in the shop
    void displayShop();

    // Private helper functions for managing the shop
    void resetShop();
    void updateShop();

    void keyPressed(const sf::Event& event);
    int whichCardClicked(sf::Vector2i mousePosition); // returns -1 id none

    void initializeDecks();
    void initializeCards();

    // Add more private members and methods as needed

};