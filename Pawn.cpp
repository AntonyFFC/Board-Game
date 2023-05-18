#include "Pawn.h"
#include "Equipment.h"

Pawn::Pawn(const std::string& name, int teamNumber, int side, int maxActions, int healthPoints, int maxEquipment, int price)
    : name(name), teamNumber(teamNumber), side(side),maxActions(maxActions), remainingActions(maxActions), HP(healthPoints), maxEquipment(maxEquipment), price(price)
{
    scaleFactor = 0.05f;
    rotationAngle = 90.0f;
    if (side == 0)
        elementsSet = { "red" };
    else if (side == 1)
        elementsSet = { "blue" };
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


void Pawn::createImage(std::unordered_set<std::string> fileNames)
{

    renderTexture = new sf::RenderTexture;
    renderTexture->create(1400, 1400);

    renderTexture->clear(sf::Color::Transparent);
    for (const auto& name : fileNames)
    {
        renderTexture->draw(spriteMap[name]);
    }
    renderTexture->display();

    combinedTexture = new sf::Texture(renderTexture->getTexture());
    /*combinedTexture->copyToImage().saveToFile("assets/combined.png");*/
    combinedSprite = new sf::Sprite(*combinedTexture);
    combinedSprite->setPosition(84.f, 504.0f);
    combinedSprite->setScale(scaleFactor, scaleFactor);
    combinedSprite->setRotation(rotationAngle);
}

void Pawn::initializeSpriteMap()
{
    std::string folderPath = "assets/";
    std::string searchPattern = folderPath + "*.png";
    WIN32_FIND_DATAA findData;
    HANDLE findHandle = FindFirstFileA(searchPattern.c_str(), &findData);

    if (findHandle != INVALID_HANDLE_VALUE)
    {
        do
        {
            std::string fileName = findData.cFileName;
            std::string filePath = folderPath + fileName;

            // Load the texture
            sf::Texture* texture;
            texture = new sf::Texture;
            if (!texture->loadFromFile(filePath))
            {
                // Handle error loading texture
                continue;
            }
            sf::Sprite sprite(*texture);
            spriteMap[fileName.substr(0,fileName.length()-4)] = sprite;

        } while (FindNextFileA(findHandle, &findData));

        FindClose(findHandle);
    }
    else
    {
        // Handle error opening directory
        return;
    }
}

void Pawn::initializePawn()
{
    initializeSpriteMap();
    createImage(elementsSet);
}

// Getter methods

std::string Pawn::getName() const {
    return name;
}

int Pawn::getTeamNumber() const {
    return teamNumber;
}

int Pawn::getSide() const {
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
    return *combinedSprite;
}

// Setter methods

void Pawn::setName(const std::string& name) {
    this->name = name;
}

void Pawn::setTeamNumber(int teamNumber) {
    this->teamNumber = teamNumber;
}

void Pawn::setSide(int side) {
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