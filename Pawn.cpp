#include "Pawn.h"
#include "Equipment.h"
#include <random>
#include <iostream>

Pawn::Pawn(const std::string& name, int teamNumber, int side, int maxActions, int healthPoints, SpaceInventory space, int price, float xPos, float yPos)
    : name(name), teamNumber(teamNumber), side(side),maxActions(maxActions), remainingActions(maxActions), HP(healthPoints), space(space), price(price), xPos(xPos), yPos(yPos), combinedSprite(), equipment()
{
    scaleFactor = 0.05f;
    rotationAngle = 90.0f;
    remainingSpace = space;
    createSprite();
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

void Pawn::createSprite()
{
    delete combinedSprite;
    std::vector<sf::Sprite> sortedSprites(order.size());
    std::unordered_set<std::string> stuff = getSet();
    for (std::string name : stuff)
    {
        sortedSprites[order.at(name)] = spriteMap[name];
    }

    sf::RenderTexture* renderTexture = new sf::RenderTexture;
    renderTexture->create(1400, 1400);

    renderTexture->clear(sf::Color::Transparent);
    for (const auto& sprite : sortedSprites)
    {
        renderTexture->draw(sprite);
    }
    renderTexture->display();

    sf::Texture *combinedTexture = new sf::Texture(renderTexture->getTexture());
    delete renderTexture;
    /*combinedTexture->copyToImage().saveToFile("assets/combined.png");*/
    combinedSprite = new sf::Sprite(*combinedTexture);
    combinedSprite->setScale(scaleFactor, scaleFactor);
    sf::Vector2f finalPos(xPos + combinedSprite->getGlobalBounds().width / 2.0f, yPos - combinedSprite->getGlobalBounds().height / 2.0f);
    combinedSprite->setPosition(finalPos.x, finalPos.y);
    combinedSprite->setRotation(rotationAngle);
}

sf::Sprite Pawn::getSprite() {
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

std::vector<Equipment*> Pawn::getEquipment() const {
    return equipment;
}

int Pawn::getPrice() const {
    return price;
}

std::tuple<int, int, int> Pawn::getHexCoords() const
{
    return hexCoords;
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
    combinedSprite->setRotation(rotationAngle);
}

void Pawn::setPosition(float inx, float iny) {
    this->xPos = inx;
    this->yPos = iny;
    sf::Vector2f finalPos(xPos + combinedSprite->getGlobalBounds().width / 2.0f, yPos - combinedSprite->getGlobalBounds().height / 2.0f);
    combinedSprite->setPosition(finalPos.x, finalPos.y);
}

void Pawn::setScale(float ins) {
    scaleFactor = ins;
    combinedSprite->setScale(scaleFactor, scaleFactor);
}

void Pawn::setHexCoords(std::tuple<int, int, int> coords)
{
    this->hexCoords = coords;
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
                createSprite();
                return true;
            }
        }
        else
        {
            if (item->getSpaceOccupied().numSpaces <= remainingSpace.extras)
            {
                equipment.push_back(item);
                remainingSpace.extras -= item->getSpaceOccupied().numSpaces;
                createSprite();
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
        createSprite();
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
    if (side == 0)
    {
        combinedSprite->setTexture(*spriteMap["dead red"].getTexture());
    }
    else
    {
        combinedSprite->setTexture(*spriteMap["dead blue"].getTexture());
    }
}

void Pawn::draw(sf::RenderTarget& target, bool isShift)
{
    target.draw(getSprite());
    if (isShift) {
        drawStats(target);
    }
}

void Pawn::drawStats(sf::RenderTarget& target)
{
    sf::Text attributesText;
    int size = 15;
    attributesText.setFont(globalFont);
    attributesText.setCharacterSize(size);

    // Position the attributes text relative to the pawn's sprite
    sf::FloatRect spriteBounds = getSprite().getGlobalBounds();
    attributesText.setPosition(spriteBounds.left + spriteBounds.width / 5, spriteBounds.top - spriteBounds.height / 4);

    attributesText.setFillColor(sf::Color::White);
    attributesText.setString(getName());
    target.draw(attributesText);

    attributesText.move(0, 15);
    attributesText.setFillColor(sf::Color::Blue);
    attributesText.setString(std::to_string(getMaxActions()));
    target.draw(attributesText);

    attributesText.move(-17, spriteBounds.height - 7);
    attributesText.setFillColor(sf::Color::Red);
    attributesText.setOutlineColor(sf::Color::Red);
    attributesText.setOutlineThickness(2);
    attributesText.setLineSpacing(0.3);
    attributesText.setCharacterSize(size * 2);
    std::string hpString = "";
    hpString = 176;
    for (int i = 0; i < getHP(); ++i) {
        attributesText.move(0, -12);
        attributesText.setString(hpString);
        target.draw(attributesText);
    }
}

void Pawn::rangedAttack(int value, int missMax) // for example if is 3 then 1,2,3 misses
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(1, 6);
    int randomNumber = dis(gen);
    if (randomNumber <= missMax)
    {
        std::cout << "miss\n";
    }
    else
    {
        attack(value);
    }
}

void Pawn::attack(int value)
{
    std::vector<bool> armours = whatArmour();
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(1, 6);
    int randomNumber = dis(gen);
    if (randomNumber == 1 && armours[2])
    {
        std::cout << "covering used\n";
        useArmour("Covering", armours, value);
    }
    else if (randomNumber > 1 && randomNumber <= 4 && armours[0])
    {
        std::cout << "shield hit\n";
        useArmour("Shield", armours, value);
    }
    else if (randomNumber <= 6 && randomNumber >= 5 && armours[1])
    {
        std::cout << "helmet hit\n";
        useArmour("Helmet", armours, value);
    }
    else if (armours[2])
    {
        std::cout << "covering used\n";
        useArmour("Covering", armours, value);
    }
    else
    {
        std::cout << "person hit\n";
        reduceHP(value);
    }
}

std::vector<bool> Pawn::whatArmour()
{
    int size = 3;
    std::vector<bool> types(size, false);
    for (Equipment* item : getEquipment())
    {
        if (item->getType() == "Armour")
        {
            if (item->getAdditionalCapabilities() == "Shield")
            {
                types[0] = true;
            }
            else if (item->getAdditionalCapabilities() == "Helmet")
            {
                types[1] = true;
            }
            else
            {
                types[2] = true;
            }
        }
    }
    return types;
}

void Pawn::useArmour(const std::string& type, std::vector<bool>& armours, int value)
{
    Equipment* armour = findArmour(type);
    int rest = armour->reduceDurability(value);
    if (armour->getAttackValue() <= 0)
    {
        auto iter = std::find(equipment.begin(), equipment.end(), armour);
        removeEquipment(std::distance(equipment.begin(), iter));
        if (armours[2] && type != "Covering")
        {
            Equipment* covering = findArmour("Covering");
            int rest2 = covering->reduceDurability(rest);
            if (covering->getAttackValue() <= 0)
            {
                auto iter = std::find(equipment.begin(), equipment.end(), covering);
                removeEquipment(std::distance(equipment.begin(), iter));
                reduceHP(rest2);
            }
        }
        else
        {
            reduceHP(rest);
        }
    }
}

Equipment* Pawn::findArmour(const std::string& type)
{
    for (Equipment* item : getEquipment())
    {
        if (item->getType() == "Armour")
        {
            if (item->getAdditionalCapabilities() == type)
            {
                return item;
            }
        }
    }
    throw std::runtime_error("Now such armour");
}

const std::map<std::string, int> Pawn::order = {
    {"red", 0},
    {"blue", 1},
    {"helmet", 2},
    {"plate armour", 3},
    {"leather armour", 3},
    {"sword", 4},
    {"shield", 5},
    {"bow", 6}
};

std::vector<sf::Texture*> Pawn::textures;

std::map<std::string, sf::Sprite> Pawn::initializeSpriteMap()
{
    std::string folderPath = "assets/equipment/";
    std::string searchPattern = folderPath + "*.png";
    WIN32_FIND_DATAA findData;
    HANDLE findHandle = FindFirstFileA(searchPattern.c_str(), &findData);

    std::map<std::string, sf::Sprite> sprites;

    if (findHandle != INVALID_HANDLE_VALUE)
    {
        do
        {
            std::string fileName = findData.cFileName;
            fileName = fileName.substr(0, fileName.length() - 4);
            std::string filePath = folderPath + fileName + ".png";

            // Load the texture
            sf::Texture* texture;
            texture = new sf::Texture;
            if (!texture->loadFromFile(filePath))
            {
                delete texture;
                continue;
            }
            // This holds the textures so that the sprites work
            textures.push_back(texture);
            sf::Sprite sprite(*texture);
            sprites[fileName] = sprite;
        } while (FindNextFileA(findHandle, &findData));
        FindClose(findHandle);
    }
    else
    {
        throw std::runtime_error("Invalid handle value");
    }
    return sprites;
}

std::map<std::string, sf::Sprite> Pawn::spriteMap = initializeSpriteMap();
