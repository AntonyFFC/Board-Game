#pragma once
#include <string>

class Equipment {
public:
    struct SpaceOccupied {
        int numSpaces;
        std::string spaceType;
    };

    Equipment(const std::string& name, int range, const SpaceOccupied& spaceOccupied, int attackValue,
        int attackActions,const std::string& type, int price, const std::string& additionalCapabilities);

    // getters
    std::string getName() const;
    int getRange() const;
    SpaceOccupied getSpaceOccupied() const;
    int getAttackValue() const;
    int getAttackActions() const;
    std::string getType() const;
    int getPrice() const;
    std::string getAdditionalCapabilities() const;

private:
    std::string name;
    int range;
    SpaceOccupied spaceOccupied;
    int attackValue;
    int attackActions;
    std::string type;
    int price;
    std::string additionalCapabilities;
};
