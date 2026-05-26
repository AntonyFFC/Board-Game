#include "ShopPawns.h"

ShopPawns::ShopPawns() {
}

ShopPawns::~ShopPawns() {
    // Perform cleanup if necessary
}

void ShopPawns::draw(sf::RenderTarget* window)
{
    sf::Vector2f pos(window->getSize().x *0.65, 130);
    for (Pawn* pawn : pawns) {
        pawn->scale(0.2f);
        pawn->setRotationAngle(0.f);
        pawn->setPosition(pos.x, pos.y);
        pawn->draw(*window, false);
        pos += sf::Vector2f(0, 0.7 * pawn->getSprite().getGlobalBounds().height);
    }
    for (Pawn* pawn : pawns) {
        pawn->drawTable(dynamic_cast<sf::RenderWindow*>(window));
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

void ShopPawns::togglePawnEquipmentTable(int pawnIndex)
{
	if (pawnIndex >= 0 && pawnIndex < pawns.size()) {
		pawns[pawnIndex]->toggleIsEquipmentShown();
	}
}

void ShopPawns::setPawnEquipmentTable(int pawnIndex, bool value)
{
	if (pawnIndex >= 0 && pawnIndex < pawns.size()) {
		pawns[pawnIndex]->setIsEquipmentShown(value);
	}

}

std::vector<Pawn*> ShopPawns::getPawns()
{
    return pawns;
}

void ShopPawns::hideAllTables()
{
	for (Pawn* pawn : pawns) {
		pawn->setIsEquipmentShown(false);
	}
}
