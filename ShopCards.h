#pragma once
#include <vector>
#include "Pawn.h"
#include "Equipment.h"
#include <SFML/Graphics.hpp>
#include "Card.h"
#include "EquipmentManager.h"
#include "PawnsManager.h"
#include <random> 


class ShopCards {
public:
    ShopCards();
    ~ShopCards();

    void draw(sf::RenderTarget* window);
    void makeCards(std::vector<Equipment*> availableItems, std::vector<Pawn*> availableWarriors);
    void updateDecks();
    int whichCardClicked(sf::Vector2i mousePosition); //returns -1 if none were clicked
    int getPriceOfCard(int cardNum);
    void clickCard(int cardNum);
    void unClickAll();
    void flipPage();

    template <typename T>
    T* getCard(int cardNum);
    void removeCard(int cardNum);

    // Add more methods as needed
    bool currentPage;
private:
    std::vector<WarriorCard*> warriorsCards; //shownWarriots
    std::vector<EquipmentCard*> itemsCards; //shownItems
    std::vector<Equipment*> equipmentList; //loaded equipment
    std::vector<Pawn*> pawnsList; //loaded pawns
};

template<typename T>
inline T* ShopCards::getCard(int cardNum)
{
    if (currentPage)
    {
        return dynamic_cast<T*>(itemsCards[cardNum]);
    }
    else
    {
        return dynamic_cast<T*>(warriorsCards[cardNum]);
    }
}
