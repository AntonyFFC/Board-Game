#include "Equipment.h"
#include "Pawn.h"
#include <stdexcept>

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

Equipment::Equipment(const std::string& name, Range baseRange, const SpaceOccupied& spaceOccupied,
    int baseAttack, int attackActions,const std::string& type, int price,
    const std::string& additionalCapabilities, const int numInDeck,
    std::vector<std::pair<std::string, int>> actionBonus,
    std::vector<std::pair<std::string, int>> attackBonus, 
    std::vector<std::pair<std::string, int>> rangeBonus)
    : name(name), baseRange(baseRange), spaceOccupied(spaceOccupied), baseAttack(baseAttack),
    attackActions(attackActions), type(type), price(price),
    additionalCapabilities(additionalCapabilities), numInDeck(numInDeck),
	actionBonus(actionBonus), attackBonus(attackBonus), rangeBonus(rangeBonus)
{
	owner = nullptr;
    ranged = getFirstWordCapabilities()=="Ranged";
    miss = 0;
    if (isRanged())
    {
        miss = getMissValueFromString(additionalCapabilities);
    }
}

std::string Equipment::getName() const {
    return name;
}

bool Equipment::doesNameContain(std::string name) const
{
	return this->name.find(name) != std::string::npos;
}

Equipment::Range Equipment::getRange() const {
    if (owner == nullptr) {
        return baseRange;
    }
    Range rangeValue = baseRange;
    for (const auto& pair : rangeBonus) {
        if ((pair.first == "isTownsman" && owner->getFirstName() == "Townsman") ||
            (pair.first == "isPrince" && owner->getFirstName() == "Prince") ||
            (pair.first == "isKnight" && owner->getFirstName() == "Knight") ||
            (pair.first == "isPeasant" && owner->getFirstName() == "Peasant") ||
            (pair.first == "hasShield" && owner->hasShield()) ||
            (pair.first == "isMounted" && owner->isMounted())) {
            rangeValue.maxRange += pair.second;
        }
    }
    return rangeValue;
}

Equipment::SpaceOccupied Equipment::getSpaceOccupied() const {
    return spaceOccupied;
}

int Equipment::getAttackValue() const {
	if (owner == nullptr) {
		return baseAttack;
	}
    int attackValue = baseAttack;
    for (const auto& pair : attackBonus) {
        if ((pair.first == "isTownsman" && owner->getFirstName() == "Townsman") ||
            (pair.first == "isPrince" && owner->getFirstName() == "Prince") || 
            (pair.first == "isKnight" && owner->getFirstName() == "Knight") ||
            (pair.first == "isPeasant" && owner->getFirstName() == "Peasant") || 
            (pair.first == "hasShield" && owner->hasShield()) || 
            (pair.first == "isMounted" && owner->isMounted())) {
            attackValue += pair.second;
        }
    }
    if ((owner->getSecondName() == "Defender") &&
        ((owner->getFirstName() == "Townsman" && getFirstWordCapabilities() == "Covering") ||
        (owner->getFirstName() == "Prince" && getFirstWordCapabilities() == "Helmet") ||
        ((owner->getFirstName() == "Peasant" || owner->getFirstName() == "Knight") &&
        getFirstWordCapabilities() == "Shield")))
    {
        attackValue += 1;
    }
    return attackValue;
}

int Equipment::getAttackActions() const {
    if (owner == nullptr) {
		return attackActions;
	}
    int actionsValue = attackActions;
    for (const auto& pair : actionBonus) {
        if ((pair.first == "isTownsman" && owner->getFirstName() == "Townsman") ||
            (pair.first == "isPrince" && owner->getFirstName() == "Prince") || 
            (pair.first == "isKnight" && owner->getFirstName() == "Knight") ||
            (pair.first == "isPeasant" && owner->getFirstName() == "Peasant") ||
            (pair.first == "hasShield" && owner->hasShield()) ||
            (pair.first == "isMounted" && owner->isMounted())) {
            actionsValue += pair.second;
        }
    }
    return actionsValue;
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

std::string Equipment::getFirstWordCapabilities() const
{
	std::istringstream iss(additionalCapabilities);
	std::string firstWord;
	iss >> firstWord;
	return firstWord;
}

int Equipment::getMissMax() const {
    return miss;
}

int Equipment::getNumInDeck() const
{
    return numInDeck;
}

std::vector<std::pair<std::string, int>> Equipment::getActionBonus() const
{
	return actionBonus;
}

std::vector<std::pair<std::string, int>> Equipment::getAttackBonus() const
{
	return attackBonus;
}

std::vector<std::pair<std::string, int>> Equipment::getRangeBonus() const
{
	return rangeBonus;
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
        baseAttack -= value;
		int attackValue = getAttackValue();
        if (attackValue < 0)
        {
            rest = -attackValue;
			baseAttack += rest; // now the getAttackValue() will return 0
        }
        return rest;
    }

}

bool Equipment::isRanged() const
{
    return ranged;
}


std::string Equipment::rangeToString() const
{
    std::string outText;
    Range rangeValue = getRange();
    outText = std::to_string(rangeValue.minRange) + "-" + std::to_string(rangeValue.maxRange);
    return outText;
}

std::string Equipment::spaceToString() const
{
    std::string outText;
    outText = std::to_string(spaceOccupied.numSpaces) + ":" + spaceOccupied.spaceType;
    return outText;
}

void Equipment::setOwner(Pawn* owner)
{
	this->owner = owner;
}

Pawn* Equipment::getOwner() const
{
    return this->owner;
}
