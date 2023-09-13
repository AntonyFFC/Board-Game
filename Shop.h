#pragma once
#include <vector>
#include <string>
#include "Equipment.h"
#include "Pawn.h"

class Shop {
public:
    // Constructor
    Shop();

    // Destructor
    ~Shop();

    // Initialize the shop with available cards and items
    void initializeShop();

    // Display the available cards and items in the shop
    void displayShop();

    // Player buys a card or item from the shop
    bool buy(int player, int card);

    // Proceed to the next turn and update the shop
    void nextTurn();

private:
    // Private member variables for shop state
    std::vector<Pawn> availableCards;
    std::vector<std::string> availableItems;
    int currentRound;
    int currentPlayerIndex;
    int remainingGold;

    // Private helper functions for managing the shop
    void resetShop();
    void updateShop();

    // Add more private members and methods as needed

};