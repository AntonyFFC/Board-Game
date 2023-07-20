#pragma once
#include <string>
#include <map>
#include <sstream>

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
        int attackActions,const std::string& type, int price, const std::string& additionalCapabilities);

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
    int reduceDurability(int value);
    bool isRanged();
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
};
