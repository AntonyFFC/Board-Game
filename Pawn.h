#pragma once

#include <string>
#include <vector>
#include "Equipment.h"
#include <SFML/Graphics.hpp>

class Side {
public:
    virtual ~Side() {}
};

class Red : public Side {
    
};

class Blue : public Side {
    
};


class Pawn {
public:
    Pawn() : name(""), teamNumber(0), side(nullptr), maxActions(0), remainingActions(0), HP(0), maxEquipment(0), price(0) {
        initializeImage();
        initializePawn();
    }
    Pawn(const std::string& name, int teamNumber, Side* side, int maxActions, int healthPoints, int maxEquipment, int price);
    ~Pawn();

    void initializeImage();
    void initializePawn();

    // Getter methods
    std::string getName() const;
    int getTeamNumber() const;
    Side* getSide() const;
    int getRemainingActions() const;
    int getMaxActions() const;
    int getHP() const;
    int getMaxEquipment() const;
    int getEquipmentCount() const;
    Equipment* getEquipment(int index) const;
    int getPrice() const;
    sf::Sprite getImage() const;

    // Setter methods
    void setName(const std::string& name);
    void setTeamNumber(const int teamNumber);
    void setSide(Side* side);
    void setRemainingActions(int actions);
    void setHP(int healthPoints);

    // Equipment-related methods
    bool addEquipment(Equipment* weapon);
    bool removeEquipment(int index);

    // Other methods
    void reduceActions(int amount);
    void reduceHP(int amount);
    bool isAlive() const;

    void dead();

    
private:
    std::string name;
    int teamNumber;
    Side* side;
    int remainingActions;
    int maxActions;
    int HP;
    int maxEquipment;
    int price;
    std::vector<Equipment*> equipment;
    sf::Sprite image;
    sf::Texture *pawnTexture;
    
    
};
