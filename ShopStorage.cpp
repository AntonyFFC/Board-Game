#include "ShopStorage.h"

ShopStorage::ShopStorage()
{
    radius = 30.f;
    rectangle.setFillColor(sf::Color(169, 169, 169));
    circle.setRadius(radius);
    circle.setFillColor(sf::Color(139, 69, 19));
    numberOfWalls = 0;
}

void ShopStorage::draw(sf::RenderTarget* target)
{
    drawRectangle(target);
    drawCards(target);
    drawWalls(target);
}

int ShopStorage::whichItemClicked(sf::Vector2i mousePosition)
{
    int i = 0;
    for (const EquipmentCard* item : storedItems) {
        if (item->isClicked(mousePosition)) {
            return i;
        }
        i++;
    }
    return -1;
}

bool ShopStorage::isClicked(sf::Vector2i mousePosition)
{
    sf::FloatRect bounds = rectangle.getGlobalBounds();
    return bounds.contains(static_cast<sf::Vector2f>(mousePosition));
}

void ShopStorage::addCard(EquipmentCard* itemCard)
{
	storedItems.push_back(itemCard);
}

EquipmentCard* ShopStorage::takeItem(int index)
{
    EquipmentCard* card = storedItems[index];
    removeItem(index);
    return card;
}

void ShopStorage::addWall()
{
    numberOfWalls++;
}

int ShopStorage::getNumberOfWalls()
{
	return numberOfWalls;
}

void ShopStorage::drawRectangle(sf::RenderTarget* target)
{
    sf::Vector2u targetSize = target->getSize();
    rectangle.setSize(sf::Vector2f(targetSize.x / 5, targetSize.y * 0.9));
    rectangle.setPosition(sf::Vector2f(targetSize.x * 0.85, targetSize.y * 0.05));
    target->draw(rectangle);
}

void ShopStorage::drawCards(sf::RenderTarget* target)
{
    sf::Vector2u targetSize = target->getSize();
    sf::Vector2f pos(targetSize.x, targetSize.y * 0.07);
    for (EquipmentCard* item : storedItems) {
        item->setPosition(sf::Vector2f(pos.x - item->getFullSprite().getGlobalBounds().width / 2, pos.y));
        target->draw(item->getFullSprite());
        pos += sf::Vector2f(0, 0.8 * item->getFullSprite().getGlobalBounds().height);
    }
}

void ShopStorage::drawWalls(sf::RenderTarget* target)
{
    sf::Vector2u targetSize = target->getSize();
    sf::Vector2f pos(targetSize.x*0.8, targetSize.y * 0.07);
    for (int i = 0; i < numberOfWalls; i++) {
        circle.setPosition(pos);
        target->draw(circle);
        pos += sf::Vector2f(0, radius*2.5);
    }
}

void ShopStorage::removeItem(int index)
{
    if (index >= 0 && index < storedItems.size()) {
        storedItems.erase(storedItems.begin() + index);
    }
}
