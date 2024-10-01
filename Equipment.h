#pragma once
#include <string>
#include <map>
#include <sstream>
#include <vector>

class Equipment {
public:
    struct SpaceOccupied {
        int numSpaces;
        std::string spaceType;
    };
    struct Range {
        int minRange;
        int maxRange;
    };

    Equipment(const std::string& name, Range range, const SpaceOccupied& spaceOccupied, int attackValue,
              int attackActions, const std::string& type, int price, const std::string& additionalCapabilities,
              const int numInDeck = 1, 
              std::vector<std::pair<std::string, int>> actionBonus = {},
              std::vector<std::pair<std::string, int>> attackBonus = {},
              std::vector<std::pair<std::string, int>> rangeBonus = {});

    // getters
    std::string getName() const;
    Range getRange() const;
    SpaceOccupied getSpaceOccupied() const;
    int getAttackValue() const;
    int getAttackActions() const;
    std::string getType() const;
    int getPrice() const;
    std::string getAdditionalCapabilities() const;
    int getMissMax() const;
    int getNumInDeck() const;
	std::vector<std::pair<std::string, int>> getActionBonus() const;
	std::vector<std::pair<std::string, int>> getAttackBonus() const;
	std::vector<std::pair<std::string, int>> getRangeBonus() const;
    int reduceDurability(int value);
    bool isRanged() const;
    std::string rangeToString() const;
    std::string spaceToString() const;
private:
    std::string name;
    Range range;
    SpaceOccupied spaceOccupied;
    int attackValue;
    int attackActions;
    std::string type;
    int price;
    std::string additionalCapabilities;
    bool ranged;
    int miss;
    int numInDeck;
    std::vector<std::pair<std::string, int>> actionBonus;
    std::vector<std::pair<std::string, int>> attackBonus;
    std::vector<std::pair<std::string, int>> rangeBonus;
};
