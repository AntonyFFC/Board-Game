#include "ShopPawns.h"

ShopPawns::ShopPawns() {
}

ShopPawns::~ShopPawns() {
    // Perform cleanup if necessary
}

void ShopPawns::draw(sf::RenderTarget* window)
{
    sf::Vector2f pos(window->getSize().x *0.45, 130);
    for (Pawn* pawn : pawns) {
        pawn->scale(0.2);
        pawn->setRotationAngle(0.f);
        pawn->setPosition(pos.x - pawn->getSprite().getGlobalBounds().width / 2, pos.y);
        pawn->draw(*window, false);
        pos += sf::Vector2f(0, 0.7 * pawn->getSprite().getGlobalBounds().height);
    }
}

int ShopPawns::whichPawnClicked(sf::Vector2i mousePosition) {
    int i = 0;
    for (const Pawn* pawn : pawns) {
        if (pawn->isClicked(mousePosition)) {
            return i;
        }
        i++;
    }
    return -1;
}

void ShopPawns::addPawn(Pawn* pawn)
{
    pawns.push_back(pawn);
}

bool ShopPawns::addEquipmentToPawn(int pawnIndex, Equipment* equipment) {
    // Check if the pawnIndex is valid
    if (pawnIndex >= 0 && pawnIndex < pawns.size()) {
        return pawns[pawnIndex]->addEquipment(equipment);
    }
    return false;
}