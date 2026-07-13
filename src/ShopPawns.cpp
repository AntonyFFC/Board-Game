#include "ShopPawns.h"

ShopPawns::ShopPawns() {
}

ShopPawns::~ShopPawns() {
}

void ShopPawns::setColumn(const sf::FloatRect& column)
{
    column_ = column;
}

bool ShopPawns::updateHover(sf::Vector2f mousePosition)
{
    bool anyHovered = false;
    for (Pawn* pawn : pawns) {
        const bool isUnderMouse = pawn->isClicked(mousePosition);
        pawn->setHovered(isUnderMouse);
        if (isUnderMouse) {
            anyHovered = true;
        }
    }
    return anyHovered;
}

void ShopPawns::draw(sf::RenderTarget* window)
{
    sf::Vector2f pos(column_.left + column_.width * 0.5f, column_.top);
    for (Pawn* pawn : pawns) {
        pawn->scale(0.2f);
        pawn->setRotationAngle(0.f);
        pawn->setPosition(pos.x, pos.y);
        pawn->draw(*window, false);
        pos += sf::Vector2f(0, 0.7f * pawn->getSprite().getGlobalBounds().height);
    }
    for (Pawn* pawn : pawns) {
        pawn->drawTable(dynamic_cast<sf::RenderWindow*>(window));
    }
}

int ShopPawns::whichPawnClicked(sf::Vector2f mousePosition) {
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
    if (pawnIndex >= 0 && pawnIndex < static_cast<int>(pawns.size())) {
        return pawns[pawnIndex]->addEquipment(equipment);
    }
    return false;
}

void ShopPawns::togglePawnEquipmentTable(int pawnIndex)
{
	if (pawnIndex >= 0 && pawnIndex < static_cast<int>(pawns.size())) {
		pawns[pawnIndex]->toggleIsEquipmentShown();
	}
}

void ShopPawns::setPawnEquipmentTable(int pawnIndex, bool value)
{
	if (pawnIndex >= 0 && pawnIndex < static_cast<int>(pawns.size())) {
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
