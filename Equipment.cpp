#include "Equipment.h"
#include <stdexcept>

bool isStringRanged(const std::string& text)
{
    std::istringstream iss(text);

    std::string firstWord;
    iss >> firstWord;
    return firstWord == "Ranged";
}

int getMissValueFromString(const std::string& text)
{
    std::istringstream iss(text);

    std::string firstWord;
    iss >> firstWord;
    int secondWordAsInt;
    if (iss >> secondWordAsInt) {
        return secondWordAsInt;
    }
    return 0;
}

Equipment::Equipment(const std::string& name, Range range, const SpaceOccupied& spaceOccupied, int attackValue,
    int attackActions,const std::string& type, int price,
    const std::string& additionalCapabilities)
    : name(name), range(range), spaceOccupied(spaceOccupied), attackValue(attackValue),
    attackActions(attackActions), type(type), price(price),
    additionalCapabilities(additionalCapabilities) 
{
    ranged = isStringRanged(additionalCapabilities);
    miss = 0;
    if (isRanged())
    {
        miss = getMissValueFromString(additionalCapabilities);
    }
}

std::string Equipment::getName() const {
    return name;
}

Equipment::Range Equipment::getRange() const {
    return range;
}

Equipment::SpaceOccupied Equipment::getSpaceOccupied() const {
    return spaceOccupied;
}

int Equipment::getAttackValue() const {
    return attackValue;
}

int Equipment::getAttackActions() const {
    return attackActions;
}

std::string Equipment::getType() const {
    return type;
}

int Equipment::getPrice() const {
    return price;
}

std::string Equipment::getAdditionalCapabilities() const {
    return additionalCapabilities;
}

int Equipment::getMissMax() const {
    return miss;
}

int Equipment::reduceDurability(int value)
{
    if (getType() != "Armour")
    {
        throw std::runtime_error("Item is not an armour");
    }
    else
    {
        int rest = 0;
        attackValue -= value;
        if (attackValue < 0)
        {
            rest = -attackValue;
            attackValue = 0;
        }
        return rest;
    }

}

bool Equipment::isRanged()
{
    return ranged;
}