#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "Equipment.h"
class EquipmentPile
{
public:
    EquipmentPile(std::tuple<int, int, int> hexCoords);
    ~EquipmentPile();

    void addEquipment(Equipment* equipment);
    void removeEquipment(Equipment* equipment);
    std::vector<Equipment*>& getEquipment();
    void setPosition(float x, float y);
    sf::Vector2f getPosition() const;
    std::tuple<int, int, int> getHexCoords() const;
    void draw(sf::RenderWindow& window);

private:
    void loadTexture();
    void setUpPosition();

    std::vector<Equipment*> equipmentList;
    sf::Sprite sprite;
    sf::Texture texture;
    float xPos;
    float yPos;
    std::tuple<int, int, int> hexCoords;
    float scaleFactor;
    float rotationAngle;
};

