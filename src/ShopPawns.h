#pragma once
#include <vector>
#include "Pawn.h"
#include "Equipment.h"
#include <SFML/Graphics.hpp>

class ShopPawns {
public:
    ShopPawns();
    ~ShopPawns();

    void setColumn(const sf::FloatRect& column);
    void draw(sf::RenderTarget* window);
    bool updateHover(sf::Vector2f mousePosition);
    int whichPawnClicked(sf::Vector2f mousePosition);
    void addPawn(Pawn* pawn);
    bool addEquipmentToPawn(int pawnIndex, Equipment* equipment);
    void togglePawnEquipmentTable(int pawnIndex);
	void setPawnEquipmentTable(int pawnIndex, bool value);
    std::vector<Pawn*> getPawns();
	void hideAllTables();

private:
    std::vector<Pawn*> pawns;
    sf::FloatRect column_;
};
