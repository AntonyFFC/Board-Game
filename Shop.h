#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include <string>
#include "Equipment.h"
#include "Pawn.h"
#include "Globals.h"
#include "Card.h"

class Shop {
public:
    Shop(sf::RenderWindow* window);
    ~Shop();

    void start();

    // Initialize the shop with available cards and items
    void initializeShop();

    // Display the available cards and items in the shop
    void displayShop();

    // Player buys a card or item from the shop
    bool buy(int player, int card);

    // Proceed to the next turn and update the shop
    void nextTurn();

private:
    sf::RenderWindow* window;
    std::vector<Card> availableCards;
    std::vector<Equipment*> equipmentList;
    std::vector<Pawn*> pawnsList;
    std::vector<std::string> availableItems;
    int currentRound;
    int currentPlayerIndex;
    int remainingGold;
    sf::Sprite backgroundSprite;
    sf::Texture backgroundTexture;
    sf::Text titleText;

    // Private helper functions for managing the shop
    void resetShop();
    void updateShop();

    // Add more private members and methods as needed

};