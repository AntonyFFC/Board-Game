#include "Pawn.h"
#include <random>
#include <iostream>

Pawn::Pawn(const std::string& name, int teamNumber, int side, int maxActions, 
    int healthPoints, SpaceInventory space, int price, int numInDeck)
    : name(name), teamNumber(teamNumber), side(side),maxActions(maxActions), 
    remainingActions(maxActions), HP(healthPoints), space(space), price(price), 
	numInDeck(numInDeck), combinedSprite(), equipment()
{
    scaleFactor = 0.05f;
    rotationAngle = 90.0f;
    remainingSpace = space;
    isEquipmentShown = false;
    equipmentTable = nullptr;
    createSprite();
    xPos = 0;
    yPos = 0;
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
    delete combinedTexture;
    delete equipmentTable;
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
    if (isAlive())
    {
        delete combinedSprite;
        std::vector<sf::Sprite> sortedSprites(order.size());
        std::unordered_set<std::string> stuff = getSet();
        for (std::string name : stuff)
        {
            sortedSprites[order.at(name)] = spriteMap[name];
        }

        sf::RenderTexture* renderTexture = new sf::RenderTexture;
        renderTexture->create(1400, 1800);

        renderTexture->clear(sf::Color::Transparent);
        for (const auto& sprite : sortedSprites)
        {
            sf::Vector2f renderTextureCenter(renderTexture->getSize().x / 2.0f, renderTexture->getSize().y / 2.0f);
            sf::Vector2f spriteCenterOffset(sprite.getLocalBounds().width / 2.0f, sprite.getLocalBounds().height / 2.0f);
            sf::Vector2f spritePosition = renderTextureCenter - spriteCenterOffset;
            sf::Transform transform;
            transform.translate(spritePosition);
            renderTexture->draw(sprite, transform);
        }
        renderTexture->display();

        combinedTexture = new sf::Texture(renderTexture->getTexture());
        delete renderTexture;
        combinedSprite = new sf::Sprite(*combinedTexture);
        setUpPosition();
    }
    else
    {
        dead();
    }
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

Pawn::SpaceInventory Pawn::getRemainingSpace() const
{
    return remainingSpace;
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

int Pawn::getNumInDeck() const
{
    return numInDeck;
}

bool Pawn::getIsEquipmentShown() const
{
	return isEquipmentShown;
}

// Setter methods

void Pawn::setIsEquipmentShown(bool isShown)
{
    isEquipmentShown = isShown;
}

void Pawn::toggleIsEquipmentShown()
{
	isEquipmentShown = !isEquipmentShown;
}

void Pawn::setName(const std::string& name) {
    this->name = name;
}

void Pawn::setTeamNumber(int teamNumber) {
    this->teamNumber = teamNumber;
}

void Pawn::setSide(int side) {
    this->side = side;
    createSprite();
}

void Pawn::setRemainingActions(int actions) {
    this->remainingActions = actions;
}

void Pawn::setHP(int healthPoints) {
    this->HP = healthPoints;
}

void Pawn::setRotationAngle(float angle) {
    this->rotationAngle = angle;
    setUpPosition();
}

void Pawn::setPosition(float inx, float iny) {
    this->xPos = inx;
    this->yPos = iny;
    setUpPosition();
}

void Pawn::scale(float ins) {
    this->scaleFactor = ins;
    setUpPosition();
}

void Pawn::setHexCoords(std::tuple<int, int, int> coords)
{
    this->hexCoords = coords;
}

void Pawn::addSpace(int hands, int extras) {
    this->space.hands += hands;
    this->space.extras += extras;
    this->remainingSpace.hands += hands;
    this->remainingSpace.extras += extras;
}

// Equipment-related methods

bool Pawn::addEquipment(Equipment* item) {
    if (equipment.size() <= space.hands+space.extras) {
        if (item->getSpaceOccupied().spaceType == "hands")
        {
            if (item->getSpaceOccupied().numSpaces <= remainingSpace.hands)
            {
                if (remainingSpace.hands < space.hands)
                {
                    flipSprite(item);
                }
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
        equipment.erase(equipment.begin() + index);
        createSprite();
        return true;
    }
    return false;
}

bool Pawn::removeEquipment(Equipment* item) {
    for (Equipment* object : equipment)
    {
        if (object == item)
        {
            if (object->getSpaceOccupied().spaceType == "hands")
            {
                remainingSpace.hands += object->getSpaceOccupied().numSpaces;
            }
            else
            {
                remainingSpace.extras += object->getSpaceOccupied().numSpaces;
            }
            equipment.erase(std::remove(equipment.begin(), equipment.end(), object), equipment.end());
            createSprite();
            return true;
        }
    }
    return false;
}

void Pawn::setUpTable(sf::RenderWindow* target)
{
	if (equipmentTable == nullptr)
	{
		equipmentTable = new Table(target);
	}
	equipmentTable->setEquipment(equipment);
	equipmentTable->setPosition(sf::Vector2f(xPos - equipmentTable->getSize().x/2, yPos + getSprite().getGlobalBounds().height / 5));
	equipmentTable->createTexture();
    target->draw(equipmentTable->getTableSprite());
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
    //delete combinedSprite;
    sf::Sprite sprite;
    sf::RenderTexture* renderTexture = new sf::RenderTexture;
    renderTexture->create(1400, 1800);
    if (side == 0)
    {
        sprite = spriteMap["dead red"];
    }
    else
    {
        sprite = spriteMap["dead blue"];
    }
    sf::Vector2f renderTextureCenter(renderTexture->getSize().x / 2.0f, renderTexture->getSize().y / 2.0f);
    sf::Vector2f spriteCenterOffset(sprite.getLocalBounds().width / 2.0f, sprite.getLocalBounds().height / 2.0f);
    sf::Vector2f spritePosition = renderTextureCenter - spriteCenterOffset;
    sf::Transform transform;
    transform.translate(spritePosition);
    renderTexture->draw(sprite, transform);
    renderTexture->display();

    combinedTexture = new sf::Texture(renderTexture->getTexture());
    delete renderTexture;
    combinedSprite = new sf::Sprite(*combinedTexture);
    setUpPosition();
}

void Pawn::draw(sf::RenderTarget& target, bool isShift)
{
    target.draw(getSprite());
    if (isShift) {
        drawStats(target);
    }
    if (isEquipmentShown)
    {
		setUpTable(dynamic_cast<sf::RenderWindow*>(&target));
    }
}

void Pawn::drawTable(sf::RenderWindow* window)
{
    if (isEquipmentShown)
    {
        window->draw(equipmentTable->getTableSprite());
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

bool Pawn::isClicked(sf::Vector2i mousePosition) const
{
    return combinedSprite->getGlobalBounds().contains(mousePosition.x, mousePosition.y);
}

void Pawn::attack(int value)
{
    std::vector<bool> armours = whatArmour();
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(1, 8);
    int randomNumber = dis(gen);
    if (randomNumber == 1 && armours[2])
    {
        std::cout << "covering used\n";
        useArmour("Covering", armours, value);
    }
    else if (randomNumber > 1 && randomNumber <= 6 && armours[0])
    {
        std::cout << "shield hit\n";
        useArmour("Shield", armours, value);
    }
    else if (randomNumber <= 8 && randomNumber >= 6 && armours[1])
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

std::string getFirstWord(const std::string& str) {
    std::istringstream iss(str);
    std::string firstWord;
    iss >> firstWord;
    return firstWord;
}

std::vector<bool> Pawn::whatArmour()
{
    int size = 3;
    std::vector<bool> types(size, false);
    for (Equipment* item : getEquipment())
    {
        if (item->getType() == "Armour")
        {
            if (getFirstWord(item->getAdditionalCapabilities()) == "Shield")
            {
                types[0] = true;
            }
            else if (getFirstWord(item->getAdditionalCapabilities()) == "Helmet")
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
        removeEquipment(armour);
        delete armour;
        if (armours[2] && type != "Covering")
        {
            Equipment* covering = findArmour("Covering");
            int rest2 = covering->reduceDurability(rest);
            if (covering->getAttackValue() <= 0)
            {
                removeEquipment(covering);
                delete covering;
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
            if (getFirstWord(item->getAdditionalCapabilities()) == type)
            {
                return item;
            }
        }
    }
    throw std::runtime_error("Now such armour");
}

void Pawn::flipSprite(Equipment* item)
{
    sf::Sprite sprite = spriteMap[item->getName()];
    sf::Sprite flippedSprite(sprite);
    sf::IntRect textureRect = sprite.getTextureRect();
    textureRect.left += textureRect.width;
    textureRect.width = -textureRect.width;
    flippedSprite.setTextureRect(textureRect);
    spriteMap[item->getName()] = flippedSprite;
}

void Pawn::setUpPosition()
{
    combinedSprite->setScale(scaleFactor, scaleFactor);
    combinedSprite->setPosition(xPos, yPos);
    combinedSprite->setOrigin(combinedSprite->getLocalBounds().width / 2, 
        combinedSprite->getLocalBounds().height / 2);
    combinedSprite->setRotation(rotationAngle);
}

const std::map<std::string, int> Pawn::order = {
    {"red", 0},
    {"blue", 0},
    {"helmet", 1},
    {"great helmet", 1},
    {"plate armour", 2},
    {"leather armour", 2},
    {"horse", 3},
    {"war horse", 3},
    {"dagger", 4},
    {"long dagger", 5},
    {"sword", 6},
    {"two handed sword", 7},
    {"light shield", 8},
    {"shield", 9},
    {"heavy shield", 10},
    {"bow", 11},
    {"longbow", 12},
    {"crossbow", 13},
    {"heavy crossbow", 14},
    {"axe", 15},
    {"two handed axe", 16},
    {"flail", 17},
    {"fork", 18},
    {"spear", 19},
    {"warhammer", 20},
    {"large slingshot", 21},
    {"slingshot", 22},
    {"gauntlets", 23},
    {"bracers", 24},
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
