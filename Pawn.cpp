#include "Pawn.h"
#include "Equipment.h"

Pawn::Pawn(const std::string& name, int teamNumber, Side* side, int maxActions, int healthPoints, int maxEquipment, int price)
    : name(name), teamNumber(teamNumber), side(side),maxActions(maxActions), remainingActions(maxActions), HP(healthPoints), maxEquipment(maxEquipment), price(price)
{
    initializeImage();
    initializePawn();
    initializespriteMap();
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
    sf::Texture* pawnTexture1;
    sf::Texture* pawnTexture2;
    pawnTexture1 = new sf::Texture;
    pawnTexture2 = new sf::Texture;
    if (!(pawnTexture1->loadFromFile("assets/helmetRed.png")))
    {
        return;
    }
    if (!(pawnTexture2->loadFromFile("assets/sword.png")))
    {
        return;
    }
    sf::Sprite image;
    sf::Sprite image2;
    image.setTexture(*pawnTexture1);
    image2.setTexture(*pawnTexture2);


    float scaleFactor = 1;
    image.setScale(scaleFactor, scaleFactor);
    float rotationAngle = 90.0f;
    //image.setRotation(rotationAngle);


    image2.setScale(scaleFactor, scaleFactor);
    //image2.setRotation(rotationAngle);

    renderTexture = new sf::RenderTexture;
    renderTexture->create(1400, 1400);

    renderTexture->clear(sf::Color::Transparent);

    // Draw the sprites onto the render texture
    renderTexture->draw(image);
    renderTexture->draw(image2);
    renderTexture->display();
    combinedTexture = new sf::Texture(renderTexture->getTexture());
    combinedTexture->copyToImage().saveToFile("assets/combined.png");
    combinedSprite = new sf::Sprite(*combinedTexture);
    combinedSprite->setPosition(500.f, 180.0f);
    combinedSprite->setScale(scaleFactor, scaleFactor);
    combinedSprite->setRotation(rotationAngle);
}

void Pawn::initializespriteMap()
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
            sf::Texture texture;
            if (!texture.loadFromFile(filePath))
            {
                // Handle error loading texture
                continue;
            }

            // Create the sprite and add it to the map
            sf::Sprite sprite(texture);
            sprite.setPosition(800.f, 180.0f);
            spriteMap[fileName] = sprite;

        } while (FindNextFileA(findHandle, &findData));

        FindClose(findHandle);
    }
    else
    {
        // Handle error opening directory
        return;
    }

    //std::vector<sf::Texture*> textures;
    //textures.push_back(new sf::Texture);
    //if (!(textures[0]->loadFromFile("assets/red.png")))
    //{
    //    return;
    //}
    //sf::Sprite image;
    //image.setTexture(*textures[0]);
    //spriteMap["red"] = image;
    ///*if (!(texture->loadFromFile("assets/sword.png")))
    //{
    //    return;
    //}
    //sf::Sprite image2;

    //image2.setTexture(*pawnTexture2);*/
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
    return *combinedSprite;
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