#pragma once

#include <string>
#include <vector>
#include "Equipment.h"
#include <SFML/Graphics.hpp>
#include <Windows.h>
#include <unordered_set>
#include <stdexcept>
#include "Globals.h"
#include "Table.h"
#include "Equipment.h"

class Table;

class Pawn {
public:
    struct SpaceInventory {
        int hands;
        int extras;
    };

    Pawn(const std::string& name, int teamNumber, int side, int maxActions, 
        int healthPoints, SpaceInventory space, int price, const int numInDeck=1);
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
    int getNumInDeck() const;
	bool getIsEquipmentShown() const;
	bool getIsInGame() const;

    // Setter methods
    void setName(const std::string& name);
    void setTeamNumber(const int teamNumber);
    void setSide(int side);
    void setRemainingActions(int actions);
    void setHP(int healthPoints);
    void setRotationAngle(float angle);
    void setPosition(float inx, float iny);
    void setIsEquipmentShown(bool isShown);
    void toggleIsEquipmentShown();
    void scale(float ins);
    void setHexCoords(std::tuple<int, int, int> coords);
    void addSpace(int hands, int extras);
	void setIsInGame(bool inGame);

    // Equipment-related methods
    bool addEquipment(Equipment* item);
    bool removeEquipment(int index);
    bool removeEquipment(Equipment* item);
    void setUpTable(sf::RenderWindow* target);

    // Other methods
    void reduceActions(int amount);
	void reduceMaxActions(int amount);
    void reduceHP(int amount);
    bool isAlive() const;
    void dead();
    void draw(sf::RenderTarget& target, bool isShift);
	void drawTable(sf::RenderWindow* window);
    void attack(int value);
    void rangedAttack(int value, int missMax);
    bool isClicked(sf::Vector2i mousePosition) const;

    static const std::map<std::string, int> order;
    static std::map<std::string, sf::Sprite> spriteMap;
    static std::vector<sf::Texture*> textures;
private:
    void handsExtrasToSet(std::unordered_set<std::string>& set);
    std::unordered_set<std::string> getSet();
    void createSprite();
    static std::map<std::string, sf::Sprite> initializeSpriteMap();
    void drawStats(sf::RenderTarget& target);
    std::vector<bool> whatArmour();
    Equipment* findArmour(const std::string& type);
    void useArmour(const std::string& type, std::vector<bool>& armours, int value);
    void flipSprite(std::string name);
    void setUpPosition();
	void calculateInitialActions();

    std::string name;
    int teamNumber;
    int side;
    int remainingActions;
	int maxActions; // The maximum number of actions a pawn can take in a turn without any items
	int calculatedMaxActions; // The maximum number of actions a pawn can take in a turn
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
    int numInDeck;
	Table* equipmentTable;
    bool isEquipmentShown;
    bool isInGame;
};
