#pragma once
#include <vector>
#include "Pawn.h"
#include "Equipment.h"
#include <SFML/Graphics.hpp>

class ShopPawns {
public:
    ShopPawns();
    ~ShopPawns();

    void draw(sf::RenderTarget* window);
    int whichPawnClicked(sf::Vector2i mousePosition);
    void addPawn(Pawn* pawn);
    bool addEquipmentToPawn(int pawnIndex, Equipment* equipment);

    // Add more methods as needed

private:
    std::vector<Pawn*> pawns;
};

