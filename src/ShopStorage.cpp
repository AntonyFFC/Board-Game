#include "ShopStorage.h"

ShopStorage::ShopStorage()
{
    radius = 30.f;
    rectangle.setFillColor(sf::Color(169, 169, 169));
    circle.setRadius(radius);
    circle.setFillColor(sf::Color(139, 69, 19));
    numberOfWalls = 0;
}

void ShopStorage::setPanel(const sf::FloatRect& panel)
{
    panel_ = panel;
    rectangle.setSize({ panel.width, panel.height });
    rectangle.setPosition(panel.left, panel.top);
}

void ShopStorage::draw(sf::RenderTarget* target)
{
    drawRectangle(target);
    drawCards(target);
    drawWalls(target);
}

int ShopStorage::whichItemClicked(sf::Vector2f mousePosition)
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

bool ShopStorage::isClicked(sf::Vector2f mousePosition)
{
    return rectangle.getGlobalBounds().contains(mousePosition);
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
    target->draw(rectangle);
}

void ShopStorage::drawCards(sf::RenderTarget* target)
{
    sf::Vector2f pos(panel_.left + panel_.width * 0.5f, panel_.top + panel_.height * 0.03f);
    for (EquipmentCard* item : storedItems) {
        item->setPosition(sf::Vector2f(pos.x - item->getFullSprite().getGlobalBounds().width / 2, pos.y));
        target->draw(item->getFullSprite());
        pos += sf::Vector2f(0, 0.8f * item->getFullSprite().getGlobalBounds().height);
    }
}

void ShopStorage::drawWalls(sf::RenderTarget* target)
{
    sf::Vector2f pos(panel_.left + panel_.width * 0.35f, panel_.top + panel_.height * 0.03f);
    for (int i = 0; i < numberOfWalls; i++) {
        circle.setPosition(pos);
        target->draw(circle);
        pos += sf::Vector2f(0, radius * 2.5f);
    }
}

void ShopStorage::removeItem(int index)
{
    if (index >= 0 && index < static_cast<int>(storedItems.size())) {
        storedItems.erase(storedItems.begin() + index);
    }
}
