#include "Pawn.h"
#include "Equipment.h"

Pawn::Pawn(const std::string& name, int teamNumber, int side, int maxActions, int healthPoints, SpaceInventory space, int price, float xPos, float yPos)
    : name(name), teamNumber(teamNumber), side(side),maxActions(maxActions), remainingActions(maxActions), HP(healthPoints), space(space), price(price), xPos(xPos), yPos(yPos), combinedSprite(), equipment()
{
    scaleFactor = 0.05f;
    rotationAngle = 90.0f;
    remainingSpace = space;
    setupPawn();
}

Pawn::~Pawn() {
    for (sf::Texture* texture : textures)
    {
        delete texture;
    }
    textures.clear();
    spriteMap.clear();

    for (Equipment* weapon : equipment) {
        delete weapon;
    }
    equipment.clear();
    delete combinedSprite;
}

void Pawn::setupPawn() {
    initializeSpriteMap();
    createSprite();
}

std::unordered_set<std::string> Pawn::getSet() {
    std::unordered_set<std::string> nameSet;
    if (side == 0)
        nameSet.insert("red");
    else
        nameSet.insert("blue");

    for (const auto& obj : equipment) {
        std::string name = obj->getName();
        nameSet.insert(name);
    }
    return nameSet;
}

std::vector<sf::Sprite> sortSprites(Pawn& pawn,std::map<std::string, sf::Sprite> map) {

    std::vector<sf::Sprite> spriteVector(pawn.order.size());
    for (const auto& pair : map)
    {
        spriteVector[pawn.order.at(pair.first)] = pair.second;
    }
    return spriteVector;
}

void Pawn::createSprite()
{
    std::vector<sf::Sprite> sortedSprites;
    sortedSprites = sortSprites(*this, spriteMap);
    sf::RenderTexture* renderTexture = new sf::RenderTexture;
    renderTexture->create(1400, 1400);

    renderTexture->clear(sf::Color::Transparent);
    for (const auto& sprite : sortedSprites)
    {
        renderTexture->draw(sprite);
    }
    renderTexture->display();

    sf::Texture *combinedTexture = new sf::Texture(renderTexture->getTexture());
    /*combinedTexture->copyToImage().saveToFile("assets/combined.png");*/
    combinedSprite = new sf::Sprite(*combinedTexture);
    combinedSprite->setScale(scaleFactor, scaleFactor);
    sf::Vector2f finalPos(xPos + combinedSprite->getGlobalBounds().width / 2.0f, yPos - combinedSprite->getGlobalBounds().height / 2.0f);
    combinedSprite->setPosition(finalPos.x, finalPos.y);
    combinedSprite->setRotation(rotationAngle);
    delete renderTexture;
}

void Pawn::initializeSpriteMap()
{
    std::unordered_set<std::string> fileNames = getSet();
    std::string folderPath = "assets/";
    std::string searchPattern = folderPath + "*.png";
    WIN32_FIND_DATAA findData;
    HANDLE findHandle = FindFirstFileA(searchPattern.c_str(), &findData);

    if (findHandle != INVALID_HANDLE_VALUE)
    {
        do
        {
            std::string fileName = findData.cFileName;
            fileName = fileName.substr(0, fileName.length() - 4);
            if (fileNames.find(fileName) == fileNames.end() || spriteMap.find(fileName) != spriteMap.end()) {
                continue; // Skip this file
            }
            std::string filePath = folderPath + fileName + ".png";

            // Load the texture
            sf::Texture* texture;
            texture = new sf::Texture;
            if (!texture->loadFromFile(filePath))
            {
                delete texture;
                continue;
            }
            textures.push_back(texture);
            sf::Sprite sprite(*texture);
            spriteMap[fileName] = sprite;
        } while (FindNextFileA(findHandle, &findData));
        FindClose(findHandle);
    }
    else
    {
        // Handle error opening directory
        return;
    }
}

sf::Sprite Pawn::getSprite() {
    setupPawn();
    return *combinedSprite;
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

Pawn::SpaceInventory Pawn::getSpace() const {
    return space;
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
    this->remainingActions = actions;
}

void Pawn::setHP(int healthPoints) {
    this->HP = healthPoints;
}

void Pawn::setRotationAngle(float angle) {
    this->rotationAngle = angle;
}

void Pawn::setPosition(float inx, float iny) {
    xPos = inx;
    yPos = iny;
}

// Equipment-related methods

bool Pawn::addEquipment(Equipment* item) {
    if (equipment.size() <= space.hands+space.extras) {
        if (item->getSpaceOccupied().spaceType == "hands")
        {
            if (item->getSpaceOccupied().numSpaces <= remainingSpace.hands)
            {
                equipment.push_back(item);
                remainingSpace.hands -= item->getSpaceOccupied().numSpaces;
                return true;
            }
        }
        else
        {
            if (item->getSpaceOccupied().numSpaces <= remainingSpace.extras)
            {
                equipment.push_back(item);
                remainingSpace.extras -= item->getSpaceOccupied().numSpaces;
                return true;
            }
        }   
    }
    return false;
}

bool Pawn::removeEquipment(int index) {
    if (remainingSpace.hands+remainingSpace.hands < space.extras+space.hands && index >= 0 && index < equipment.size()) {
        if (equipment[index]->getSpaceOccupied().spaceType == "hands")
        {
            remainingSpace.hands += equipment[index]->getSpaceOccupied().numSpaces;
        }
        else
        {
            remainingSpace.extras += equipment[index]->getSpaceOccupied().numSpaces;
        }
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

void Pawn::changePos(float x, float y) {
    xPos = x;
    yPos = y;
}

void Pawn::dead()
{

}

const std::map<std::string, int> Pawn::order = {
    {"red", 0},
    {"blue", 1},
    {"helmet", 2},
    {"sword", 3},
    {"shield", 4}
};