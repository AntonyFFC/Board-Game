#include "Card.h"

Card::Card(const std::vector<int>& widths, const std::vector<std::string>& headers) 
	: cellWidths(widths), headers(headers)
{
}

Card::~Card()
{
}

template <typename RenderType>
void Card::draw(RenderType* window)
{
	window->draw(sprite);
}

sf::Sprite Card::getSprite()
{
	return sprite;
}

WarriorCard::WarriorCard(Pawn* warrior)
	: Card({ 250, 50, 50, 50, 50, 50 },{ "Name", "history-icon-white",
		"hand-line-icon-white", "plus-round-line-icon-white", 
		"heart-line-icon-white", "dollar-icon-white" }), warrior(warrior)
{
	texture.create(500, 300);
}

EquipmentCard::EquipmentCard(Equipment* item)
	: Card({ 150, 60, 60, 50, 50, 50, 50, 500 }, 
	{ "Name","left-right-arrow-icon-white","circle-line-icon-white","bomb-blast-icon-white",
	"history-icon-white","cube-icon-white","dollar-icon-white","Other" }), item(item)
{
	texture.create(500, 300);
}
