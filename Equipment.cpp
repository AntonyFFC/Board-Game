#include "Equipment.h"

Equipment::Equipment(const std::string& name, int range, int spaceOccupied, int attackValue,
    int attackActions,const std::string& type, int price,
    const std::string& additionalCapabilities)
    : name(name), range(range), spaceOccupied(spaceOccupied), attackValue(attackValue),
    attackActions(attackActions), type(type), price(price),
    additionalCapabilities(additionalCapabilities) {}

std::string Equipment::getName() const {
    return name;
}

int Equipment::getRange() const {
    return range;
}

int Equipment::getSpaceOccupied() const {
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