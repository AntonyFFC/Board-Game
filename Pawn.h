#pragma once

#include <string>
#include <vector>
#include "Equipment.h"
#include <SFML/Graphics.hpp>
#include <Windows.h>
#include <unordered_set>
#include <stdexcept>
#include "Globals.h"

class Pawn {
public:
    struct SpaceInventory {
        int hands;
        int extras;
    };

    Pawn(const std::string& name, int teamNumber, int side, int maxActions, int healthPoints, SpaceInventory space, int price);
    ~Pawn();


    // Getter methods
    std::string getName() const;
    int getTeamNumber() const;
    int getSide() const;
    int getRemainingActions() const;
    int getMaxActions() const;
    int getHP() const;
    SpaceInventory getSpace() const;
    SpaceInventory getRemainingSpace() const;
    int getEquipmentCount() const;
    std::vector<Equipment*> getEquipment() const;
    int getPrice() const;
    sf::Sprite getSprite();
    std::tuple<int, int, int> getHexCoords() const;

    // Setter methods
    void setName(const std::string& name);
    void setTeamNumber(const int teamNumber);
    void setSide(int side);
    void setRemainingActions(int actions);
    void setHP(int healthPoints);
    void setRotationAngle(float angle);
    void setPosition(float inx, float iny);
    void scale(float ins);
    void setHexCoords(std::tuple<int, int, int> coords);
    void addSpace(int hands, int extras);

    // Equipment-related methods
    bool addEquipment(Equipment* item);
    bool removeEquipment(int index);
    bool removeEquipment(Equipment* item);

    // Other methods
    void reduceActions(int amount);
    void reduceHP(int amount);
    bool isAlive() const;
    void dead();
    void draw(sf::RenderTarget& target, bool isShift);
    void attack(int value);
    void rangedAttack(int value, int missMax);

    static const std::map<std::string, int> order;
    static std::map<std::string, sf::Sprite> spriteMap;
    static std::vector<sf::Texture*> textures;
private:
    std::unordered_set<std::string> getSet();
    void createSprite();
    static std::map<std::string, sf::Sprite> initializeSpriteMap();
    void drawStats(sf::RenderTarget& target);
    std::vector<bool> whatArmour();
    Equipment* findArmour(const std::string& type);
    void useArmour(const std::string& type, std::vector<bool>& armours, int value);
    void flipSprite(Equipment* item);
    void setUpPosition();

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
    sf::Texture* combinedTexture;
    float xPos;
    float yPos;
    std::tuple<int, int, int> hexCoords;
};
