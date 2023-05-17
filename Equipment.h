#pragma once
#include <string>

class Type {
public:
    virtual ~Type() {}
};

class MiscellaneousType : public Type {
    // Additional properties or methods specific to MiscellaneousType
};

class WeaponType : public Type {
    // Additional properties or methods specific to WeaponType
};

class ArmorType : public Type {
    // Additional properties or methods specific to ArmorType
};

class Equipment {
public:
    Equipment(const std::string& name, int range, int spaceOccupied, int attackValue,
        int attackActions, Type* type, int price,
        const std::string& additionalCapabilities);

    std::string getName() const;
    int getRange() const;
    int getSpaceOccupied() const;
    int getAttackValue() const;
    int getAttackActions() const;
    Type* getType() const;
    int getPrice() const;
    std::string getAdditionalCapabilities() const;

private:
    std::string name;
    int range;
    int spaceOccupied;
    int attackValue;
    int attackActions;
    Type* type;
    int price;
    std::string additionalCapabilities;
};
