#include "Card.h"

Card::~Card()
{
}

template <typename RenderType>
void Card::draw(RenderType& window)
{
	window.draw(sprite);
}

WarriorCard::WarriorCard(Pawn* warrior)
	:warrior(warrior)
{
	
}

EquipmentCard::EquipmentCard(Equipment* item)
	:item(item)
{
}
