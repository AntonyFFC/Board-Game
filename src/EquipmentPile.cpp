#include "EquipmentPile.h"

EquipmentPile::EquipmentPile(std::tuple<int, int, int> hexCoords)
	: hexCoords(hexCoords), scaleFactor(0.05f), rotationAngle(0.0f), xPos(0.0f), yPos(0.0f)
{

    loadTexture();
    sprite.setTexture(texture);
    setUpPosition();
}

EquipmentPile::~EquipmentPile()
{
    // Cleanup if necessary
}

void EquipmentPile::loadTexture()
{
    if (!texture.loadFromFile("assets/equipment/equipment pile.png"))
    {
        throw std::runtime_error("Failed to load equipment pile texture");
    }
}

void EquipmentPile::addEquipment(Equipment* equipment)
{
    equipmentList.push_back(equipment);
}

void EquipmentPile::removeEquipment(Equipment* equipment)
{
    auto it = std::find(equipmentList.begin(), equipmentList.end(), equipment);
    if (it != equipmentList.end())
    {
        equipmentList.erase(it);
    }
}

std::vector<Equipment*>& EquipmentPile::getEquipment()
{
    return equipmentList;
}

void EquipmentPile::setPosition(float x, float y)
{
    xPos = x;
    yPos = y;
    setUpPosition();
}

void EquipmentPile::setUpPosition()
{
    sprite.setScale(scaleFactor, scaleFactor);
    sprite.setPosition(xPos, yPos);
    sprite.setOrigin(sprite.getLocalBounds().width / 2, sprite.getLocalBounds().height / 2);
    sprite.setRotation(rotationAngle);
}

sf::Vector2f EquipmentPile::getPosition() const
{
    return sf::Vector2f(xPos, yPos);
}

std::tuple<int, int, int> EquipmentPile::getHexCoords() const
{
    return hexCoords;
}

void EquipmentPile::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
}
