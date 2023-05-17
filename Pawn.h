#pragma once

#include <string>
#include <vector>
#include "Equipment.h"
#include <SFML/Graphics.hpp>

class Pawn {
public:
    Pawn(const std::string& name, int teamNumber, int maxActions, int healthPoints, int maxEquipment);
    ~Pawn();

    // Getter methods
    std::string getName() const;
    int getTeamNumber() const;
    int getRemainingActions() const;
    int getMaxActions() const;
    int getHP() const;
    int getMaxEquipment() const;
    int getEquipmentCount() const;
    Equipment* getEquipment(int index) const;

    // Setter methods
    void setName(const std::string& name);
    void setTeamNumber(const int teamNumber);
    void setRemainingActions(int actions);
    void setHP(int healthPoints);

    // Equipment-related methods
    bool addEquipment(Equipment* weapon);
    bool removeEquipment(int index);

    // Other methods
    void reduceActions(int amount);
    void reduceHP(int amount);
    bool isAlive() const;

private:
    std::string name;
    int teamNumber;
    int remainingActions;
    int maxActions;
    int HP;
    int maxEquipment;
    std::vector<Equipment*> equipment;
    sf::ConvexShape shape;
};
