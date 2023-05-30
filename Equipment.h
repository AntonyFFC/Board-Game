#pragma once
#include <string>

class Equipment {
public:
    Equipment(const std::string& name, int range, int spaceOccupied, int attackValue,
        int attackActions,const std::string& type, int price, const std::string& additionalCapabilities);

    std::string getName() const;
    int getRange() const;
    int getSpaceOccupied() const;
    int getAttackValue() const;
    int getAttackActions() const;
    std::string getType() const;
    int getPrice() const;
    std::string getAdditionalCapabilities() const;

private:
    std::string name;
    int range;
    int spaceOccupied;
    int attackValue;
    int attackActions;
    std::string type;
    int price;
    std::string additionalCapabilities;
};
