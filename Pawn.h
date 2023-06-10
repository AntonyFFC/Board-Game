#pragma once

#include <string>
#include <vector>
#include "Equipment.h"
#include <SFML/Graphics.hpp>
#include <Windows.h>
#include <unordered_set>

class Pawn {
public:
    struct SpaceInventory {
        int hands;
        int extras;
    };

    Pawn(const std::string& name, int teamNumber, int side, int maxActions, int healthPoints, SpaceInventory space, int price, float xPos, float yPos);
    ~Pawn();


    // Getter methods
    void setupPawn();
    std::string getName() const;
    int getTeamNumber() const;
    int getSide() const;
    int getRemainingActions() const;
    int getMaxActions() const;
    int getHP() const;
    SpaceInventory getSpace() const;
    int getEquipmentCount() const;
    Equipment* getEquipment(int index) const;
    int getPrice() const;
    sf::Sprite getSprite();

    // Setter methods
    void setName(const std::string& name);
    void setTeamNumber(const int teamNumber);
    void setSide(int side);
    void setRemainingActions(int actions);
    void setHP(int healthPoints);
    void setRotationAngle(float angle);
    void setPosition(float inx, float iny);

    // Equipment-related methods
    bool addEquipment(Equipment* item);
    bool removeEquipment(int index);

    // Other methods
    void reduceActions(int amount);
    void reduceHP(int amount);
    bool isAlive() const;

    void changePos(float x, float y);
    void dead();

    std::map<std::string, sf::Sprite> spriteMap;
private:
    std::unordered_set<std::string> getSet();
    void createSprite();
    void initializeSpriteMap();

    std::string name;
    int teamNumber;
    int side;
    int remainingActions;
    int maxActions;
    int HP;
    SpaceInventory space;
    SpaceInventory remainingSpace;
    int price;
    std::vector<Equipment*> equipment;
    float scaleFactor;
    float rotationAngle;
    mutable sf::Sprite *combinedSprite;
    float xPos;
    float yPos;
};
