#include "Pawn.h"
#include "Equipment.h"

Pawn::Pawn(const std::string& name, int teamNumber, Side* side, int maxActions, int healthPoints, int maxEquipment, int price)
    : name(name), teamNumber(teamNumber), side(side),maxActions(maxActions), remainingActions(maxActions), HP(healthPoints), maxEquipment(maxEquipment), price(price)
{
    initializeImage();
    initializePawn();
}

Pawn::~Pawn() {
    // Perform any necessary cleanup here
    // (e.g., deleting dynamically allocated resources)

    // Clear the equipment vector
    for (Equipment* weapon : equipment) {
        delete weapon;
    }
    equipment.clear();
}


void Pawn::initializeImage()
{
    
    if (!texture.loadFromFile("assets/sword.png"))
    {
        return;
    }
    image.setTexture(texture);

    image.setPosition(500.f, 200.0f);
    float scaleFactor = 0.2f;
    image.setScale(scaleFactor, scaleFactor);
    float rotationAngle = 90.0f;
    image.setRotation(rotationAngle);
}

void Pawn::initializePawn()
{
    return;
}

// Getter methods

std::string Pawn::getName() const {
    return name;
}

int Pawn::getTeamNumber() const {
    return teamNumber;
}

Side* Pawn::getSide() const {
    return side;
}

int Pawn::getRemainingActions() const {
    return remainingActions;
}

int Pawn::getMaxActions() const {
    return maxActions;
}

int Pawn::getHP() const {
    return HP;
}

int Pawn::getMaxEquipment() const {
    return maxEquipment;
}

int Pawn::getEquipmentCount() const {
    return equipment.size();
}

Equipment* Pawn::getEquipment(int index) const {
    if (index >= 0 && index < equipment.size()) {
        return equipment[index];
    }
    return nullptr;
}

int Pawn::getPrice() const {
    return price;
}

sf::Sprite Pawn::getImage() const {
    return image;
}

// Setter methods

void Pawn::setName(const std::string& name) {
    this->name = name;
}

void Pawn::setTeamNumber(int teamNumber) {
    this->teamNumber = teamNumber;
}

void Pawn::setSide(Side* side) {
    this->side = side;
}

void Pawn::setRemainingActions(int actions) {
    remainingActions = actions;
}

void Pawn::setHP(int healthPoints) {
    this->HP = healthPoints;
}

// Equipment-related methods

bool Pawn::addEquipment(Equipment* weapon) {
    if (equipment.size() < maxEquipment) {
        equipment.push_back(weapon);
        return true;
    }
    return false;
}

bool Pawn::removeEquipment(int index) {
    if (index >= 0 && index < equipment.size()) {
        delete equipment[index];
        equipment.erase(equipment.begin() + index);
        return true;
    }
    return false;
}

// Other methods

void Pawn::reduceActions(int amount) {
    remainingActions -= amount;
    if (remainingActions < 0) {
        remainingActions = 0;
    }
}

void Pawn::reduceHP(int amount) {
    HP -= amount;
    if (!isAlive())
        dead();
}

bool Pawn::isAlive() const {
    return HP > 0;
}

void Pawn::dead()
{

}