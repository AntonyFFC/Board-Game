#include "Pawn.h"
#include "Equipment.h"

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
    combinedSprite->setRotation(rotationAngle);
}

void Pawn::setPosition(float inx, float iny) {
    xPos = inx;
    yPos = iny;
    sf::Vector2f finalPos(xPos + combinedSprite->getGlobalBounds().width / 2.0f, yPos - combinedSprite->getGlobalBounds().height / 2.0f);
    combinedSprite->setPosition(finalPos.x, finalPos.y);
}

void Pawn::setScale(float ins) {
    scaleFactor = ins;
    combinedSprite->setScale(scaleFactor, scaleFactor);
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

}

void Pawn::draw(sf::RenderTarget& target, bool isShift)
{
    target.draw(getSprite());
    if (isShift) {
        sf::Text attributesText;
        attributesText.setFont(globalFont);
        attributesText.setCharacterSize(15);
        attributesText.setFillColor(sf::Color::White);

        // Set the attributes text content
        std::string attributes = getName() + "\n"
            + "A: " + std::to_string(getMaxActions()) + "\n"
            + "HP: " + std::to_string(getHP()) + "\n";

        attributesText.setString(attributes);
        float lineSpacing = 0.8f;
        attributesText.setLineSpacing(lineSpacing);

        // Position the attributes text relative to the pawn's sprite
        sf::FloatRect spriteBounds = getSprite().getGlobalBounds();
        attributesText.setPosition(spriteBounds.left, spriteBounds.top - 20);

        // Draw the attributes text
        target.draw(attributesText);
    }
}

const std::map<std::string, int> Pawn::order = {
    {"red", 0},
    {"blue", 1},
    {"helmet", 2},
    {"plate armour", 3},
    {"leather armour", 3},
    {"sword", 4},
    {"shield", 5}
};

std::vector<sf::Texture*> Pawn::textures;

std::map<std::string, sf::Sprite> Pawn::initializeSpriteMap()
{
    std::string folderPath = "assets/";
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
