#include "Pawn.h"
#include "Equipment.h"
#include <algorithm>
#include <random>
#include <iostream>

Pawn::Pawn(const std::string& name, int teamNumber, int side, int maxActions, 
    int healthPoints, SpaceInventory space, int price,
    const std::string& additionalCapabilities, int numInDeck)
	: name(name), teamNumber(teamNumber), side(side), remainingActions(maxActions), 
    maxActions(maxActions), calculatedMaxActions(maxActions), HP(healthPoints), 
    space(space), price(price), equipment(), combinedSprite(),
    additionalCapabilities(additionalCapabilities), numInDeck(numInDeck),
    dropButton(sf::Vector2f(0, 0), sf::Vector2f(150, 25), "drop selected equipment (1AP)")
{
    scaleFactor = 0.05f;
    rotationAngle = 90.0f;
    remainingSpace = space;
    isEquipmentShown = false;
	isInGame = false;
	isCurrentPawn = false;
    equipmentTable = nullptr;
    createSprite();
    xPos = 0;
    yPos = 0;
	dropButton.setTextSize(15);
    dropButton.setSizeToText();
	dropButton.setBackgroundColor(sf::Color::Red);
    floatingTexts.reserve(4);
}

Pawn::~Pawn() {
    for (sf::Texture* texture : textures)
    {
        delete texture;
    }
    textures.clear();
    spriteMap.clear();

    delete equipmentTable;

    for (Equipment* weapon : equipment) {
        delete weapon;
    }
    equipment.clear();
    delete combinedSprite;
    delete combinedTexture;

    for (auto* text : floatingTexts) {
        delete text;
    }
    floatingTexts.clear();
}

void Pawn::handsExtrasToSet(std::unordered_set<std::string>& set)
{
    if (remainingSpace.hands == 1)
    {
		set.insert("one hand");
    }
    else if (remainingSpace.hands == 2)
    {
        set.insert("two hands");
    }

	if (remainingSpace.extras == 1)
	{
		set.insert("one extra");
	}
	else if (remainingSpace.extras == 2)
	{
		set.insert("two extras");
	}
}

std::unordered_set<std::string> Pawn::getSet() {
    std::unordered_set<std::string> nameSet;
    if (side == 0)
        nameSet.insert("red");
    else
        nameSet.insert("blue");

	handsExtrasToSet(nameSet);

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
            const std::string spriteKey = flippedItems.count(name)
                ? name + " flipped"
                : name;
            sortedSprites[order.at(name)] = spriteMap[spriteKey];
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

void Pawn::createTeamShield(float size, float x, float y)
{
    teamShield.setPointCount(5);

    float shieldWidth = size * 0.6f;
    float shieldHeight = size * 0.9f;

    teamShield.setPoint(0, sf::Vector2f(x, y)); // Top-left
    teamShield.setPoint(1, sf::Vector2f(x + shieldWidth, y)); // Top-right
    teamShield.setPoint(2, sf::Vector2f(x + shieldWidth, y + shieldHeight * 0.6f)); // Mid-right
    teamShield.setPoint(3, sf::Vector2f(x + shieldWidth / 2, y + shieldHeight)); // Bottom-center
    teamShield.setPoint(4, sf::Vector2f(x, y + shieldHeight * 0.6f)); // Mid-left

    teamShield.setFillColor(getTeamColor(this->getTeamNumber()));
}

sf::Sprite Pawn::getSprite() {
    return *combinedSprite;
}

// Getter methods

std::string Pawn::getName() const {
    return name;
}

std::string Pawn::getFirstName() const
{
    std::istringstream stream(name);
    std::string word;
    stream >> word;
	return word;
}

std::string Pawn::getSecondName() const
{
    std::istringstream stream(name);
    std::string word;
    int wordCount = 0;

    while (stream >> word) {
        wordCount++;
        if (wordCount == 2) {
            return word;
        }
    }
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
    return calculatedMaxActions;
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

std::vector<Equipment*>& Pawn::getEquipment() {
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

bool Pawn::getIsInGame() const
{
    return isInGame;
}

int Pawn::getMissMax(std::string weaponName) const
{
	int missMax = 0;
	for (Equipment* item : equipment)
	{
		if (item->getName() == weaponName)
		{
			missMax = item->getMissMax();
		}
	}
	if (getSecondName() == "Archer")
	{
		missMax -= 1;
	}
    if (hasItem("bracers"))
    {
		missMax -= 1;
    }
	return missMax;
}

int Pawn::getWallDestroyCost() const
{
	if (hasItem("warhammer"))
	{
		return 1;
	}
    return 4;
}

bool Pawn::getIsCurrentPawn() const
{
	return isCurrentPawn;
}

std::vector<Equipment*> Pawn::getHighlightedEquipment() const
{
	std::vector<Equipment*> highlightedEquipment;
	if (equipmentTable != nullptr)
	{
		highlightedEquipment = equipmentTable->getHighlightedItems();
	}
	return highlightedEquipment;

}

std::string Pawn::getAdditionalCapabilities() const
{
    return additionalCapabilities;
}

// Setter methods

void Pawn::setIsEquipmentShown(bool isShown, bool isCurrent)
{
    this->isEquipmentShown = isShown;
    this->isCurrentPawn = isCurrent;
}

void Pawn::toggleIsEquipmentShown(bool isCurrent)
{
    this->isEquipmentShown = !isEquipmentShown;
    this->isCurrentPawn = isCurrent;
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

void Pawn::setIsInGame(bool inGame)
{
	isInGame = inGame;
	if (inGame) //the pawn joins the game
	{
		remainingActions = calculatedMaxActions;
	}
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
                    flippedItems.insert(item->getName());
                }
                equipment.push_back(item);
				item->setOwner(this);
                remainingSpace.hands -= item->getSpaceOccupied().numSpaces;
				calculateInitialActions();
                createSprite();
                return true;
            }
        }
        else
        {
            if (item->getSpaceOccupied().numSpaces <= remainingSpace.extras)
            {
                equipment.push_back(item);
                item->setOwner(this);
                remainingSpace.extras -= item->getSpaceOccupied().numSpaces;
                calculateInitialActions();
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
        std::string removedName = equipment[index]->getName();
        equipment.erase(equipment.begin() + index);
        clearFlipIfNoLonger(removedName);
        calculateInitialActions();
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
            std::string removedName = object->getName();
            equipment.erase(std::remove(equipment.begin(), equipment.end(), object), equipment.end());
            clearFlipIfNoLonger(removedName);
            calculateInitialActions();
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
}

// Other methods

void Pawn::reduceActions(int amount) {
    remainingActions -= amount;
    if (remainingActions < 0) {
        remainingActions = 0;
    }
}

void Pawn::reduceMaxActions(int amount)
{
	maxActions -= amount;
}

void Pawn::reduceHP(int amount) {
	int previousHP = HP;
    HP -= amount;

    addFloatingText("-" + std::to_string(amount), sf::Color::Red);

    if (combinedSprite)
        combinedSprite->setColor(sf::Color(255, 100, 100));
    damageTintTimer = 0.5f;

    if (!isAlive())
        dead();
    else if (previousHP > 2)
    {
		int reduction = 0;
		if (HP == 2)
			reduction = 1;
		else if (HP == 1)
			reduction = 2;
		if (reduction > 0)
		{
			reduceMaxActions(reduction);
			showActionPenalty(reduction);
		}
		calculateInitialActions();
    }
    else if (previousHP == 2 && HP == 1)
    {
        reduceMaxActions(1);
        showActionPenalty(1);
        calculateInitialActions();
    }
}

void Pawn::showActionPenalty(int amount)
{
    addFloatingText("-" + std::to_string(amount),
        sf::Color::Blue, 4.0f, 1.0f);
}

void Pawn::addFloatingText(const std::string& text, const sf::Color& color,
    float duration, float delay)
{
    if (delay > 0.0f)
    {
        pendingFloatingTexts.push_back({text, color, duration, delay});
        return;
    }
    sf::Vector2f pos = getSprite().getPosition();
    pos.x += 10.0f * static_cast<float>(floatingTexts.size());
    floatingTexts.push_back(new TextDamage(text, pos, color, duration));
}

bool Pawn::isAlive() const {
    return HP > 0;
}

void Pawn::dead()
{
	setIsEquipmentShown(false);
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

namespace {
    const sf::Color kPawnHoverTint{255, 248, 220};
    constexpr float kBodyVisualWidth = 1243.f;
    constexpr float kBodyVisualHeight = 1034.f;
    constexpr float kBodyOutlinePadding = 1.02f;
}

void Pawn::setHovered(bool hovered)
{
    this->hovered = hovered;
}

bool Pawn::isHovered() const
{
    return hovered;
}

void Pawn::drawHoverOutline(sf::RenderTarget& target) const
{
    if (!combinedSprite) {
        return;
    }

    const sf::Vector2f scale = combinedSprite->getScale();
    const float radiusX = (kBodyVisualWidth * 0.5f) * std::abs(scale.x) * kBodyOutlinePadding;
    const float radiusY = (kBodyVisualHeight * 0.5f) * std::abs(scale.y) * kBodyOutlinePadding;

    constexpr float unitRadius = 1.f;
    sf::CircleShape ring(unitRadius, 48);
    ring.setOrigin(unitRadius, unitRadius);
    ring.setScale(radiusX, radiusY);
    ring.setRotation(combinedSprite->getRotation());
    ring.setPosition(combinedSprite->getPosition());
    ring.setFillColor(sf::Color::Transparent);
    ring.setOutlineColor(sf::Color(255, 220, 100, 220));
    ring.setOutlineThickness(2.0f / std::min(radiusX, radiusY));
    target.draw(ring);
}

void Pawn::draw(sf::RenderTarget& target, bool isShift)
{
    sf::Sprite display = *combinedSprite;
    if (hovered) {
        display.setColor(kPawnHoverTint);
    }
    target.draw(display);
    if (hovered) {
        drawHoverOutline(target);
    }
    if (isShift) {
        drawStats(target);
    }
    if (this->isEquipmentShown)
    {
        if (!isInGame)
        {
            setUpTable(dynamic_cast<sf::RenderWindow*>(&target));
            drawStats(target);
        }
        else
        {
            if (isCurrentPawn)
            {
				dropButton.setPosition(sf::Vector2f(xPos - 50, yPos - 100));
				dropButton.draw(dynamic_cast<sf::RenderWindow&>(target));
            }
            setUpTable(dynamic_cast<sf::RenderWindow*>(&target));
        }
    }
	drawFloatingTexts(target);
}

void Pawn::drawTable(sf::RenderWindow* window)
{
    if (this->isEquipmentShown)
    {
        window->draw(equipmentTable->getTableSprite());
    }
}

void Pawn::drawStats(sf::RenderTarget& target)
{
    const sf::FloatRect spriteBounds = getSprite().getGlobalBounds();

    const float size = isInGame ? 15.0f : 40.0f;
    const sf::Vector2f namePos = isInGame
        ? sf::Vector2f(spriteBounds.left + spriteBounds.width / 5,
                       spriteBounds.top - spriteBounds.height / 4)
        : sf::Vector2f(spriteBounds.left,
                       spriteBounds.top + spriteBounds.height / 6);
    const float hpXOffset = isInGame ? 17.0f : 38.0f;
    const float hpYOffset = isInGame ? (spriteBounds.height - 7.0f)
                                     : (spriteBounds.height / 2.8f);
    const float hpOutlineThickness = isInGame ? 0.3f : 4.0f;

    drawNameAndShield(target, namePos, size);
    drawMaxActionsLine(target, namePos, size);
    if (!isInGame) drawCapabilities(target, namePos, size);
    drawHpHearts(target, namePos, size, hpXOffset, hpYOffset, hpOutlineThickness);
}

int Pawn::getHpActionPenalty() const
{
    if (HP == 2) return 1;
    if (HP == 1) return 2;
    return 0;
}

void Pawn::drawNameAndShield(sf::RenderTarget& target, sf::Vector2f namePos, float size)
{
    sf::Text text;
    text.setFont(globalFont);
    text.setCharacterSize(size);
    text.setPosition(namePos);
    text.setFillColor(sf::Color::White);
    text.setString(getName());
    target.draw(text);

    sf::FloatRect nameBounds = text.getGlobalBounds();
    float shieldX = nameBounds.left + 1.02f * nameBounds.width;
    float shieldY = nameBounds.top + 0.05f * nameBounds.height;
    createTeamShield(size, shieldX, shieldY);
    target.draw(teamShield);
}

void Pawn::drawMaxActionsLine(sf::RenderTarget& target, sf::Vector2f namePos, float size)
{
    sf::Text text;
    text.setFont(globalFont);
    text.setCharacterSize(size);
    text.setPosition(namePos.x, namePos.y + size);
    text.setFillColor(sf::Color::Blue);
    text.setString(std::to_string(getMaxActions()));
    target.draw(text);

    int penalty = getHpActionPenalty();
    if (penalty > 0)
    {
        float numberWidth = text.getGlobalBounds().width;
        text.setPosition(namePos.x + numberWidth + size * 0.2f, namePos.y + size);
        text.setFillColor(sf::Color::Red);
        text.setString("(-" + std::to_string(penalty) + ")");
        target.draw(text);
    }
}

void Pawn::drawCapabilities(sf::RenderTarget& target, sf::Vector2f namePos, float size)
{
    sf::Text text;
    text.setFont(globalFont);
    text.setCharacterSize(size / 2);
    text.setPosition(namePos.x + size, namePos.y + size);
    text.setFillColor(sf::Color::White);
    text.setString(getAdditionalCapabilities());
    target.draw(text);
}

void Pawn::drawHpHearts(sf::RenderTarget& target, sf::Vector2f namePos, float size,
    float hpXOffset, float hpYOffset, float outlineThickness)
{
    sf::Text text;
    text.setFont(globalFont);
    text.setCharacterSize(size * 2);
    text.setPosition(namePos.x - hpXOffset, namePos.y + size + hpYOffset);
    text.setFillColor(sf::Color::Red);
    text.setOutlineColor(sf::Color::Red);
    text.setOutlineThickness(2.0f * size / 15.0f);
    text.setLineSpacing(outlineThickness);

    std::string hpString;
    hpString = 176;
    for (int i = 0; i < getHP(); ++i)
    {
        text.move(0.0f, -12.0f * size / 15.0f);
        text.setString(hpString);
        target.draw(text);
    }
}

void Pawn::drawFloatingTexts(sf::RenderTarget& target)
{
    if (hasActiveAnimation()) {
        for (const auto& text : floatingTexts) {
            text->draw(target);
        }
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
        addFloatingText(std::to_string(0), sf::Color::White);
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

bool Pawn::hasItem(const std::string& name) const
{
	for (Equipment* item : equipment)
	{
		if (item->getName() == name)
		{
			return true;
		}
	}
    return false;
}

bool Pawn::hasShield() const
{
	for (Equipment* item : equipment)
	{
		if (item->getFirstWordCapabilities() == "Shield")
		{
			return true;
		}
	}
    return false;
}

bool Pawn::isMounted() const
{
	for (Equipment* item : equipment)
	{
		if (item->doesNameContain("horse"))
		{
			return true;
		}
	}
    return false;
}

bool Pawn::isEquipmentTableClicked(sf::Vector2i mousePosition) const
{
	if (equipmentTable != nullptr)
	{
		return equipmentTable->tableClicked(mousePosition);
	}
	return false;
}

bool Pawn::clickDropButton(sf::Vector2i mousePosition)
{
    return dropButton.click(mousePosition);
}

bool Pawn::unClickDropButton(sf::Vector2i mousePosition)
{
    return dropButton.unclick();
}

void Pawn::toggleHighlightEquipmentTable(sf::Vector2i mousePosition)
{
	equipmentTable->toggleHighlightClickeditem(mousePosition);
}

bool Pawn::areAnyHighlighted() const
{
	if (equipmentTable != nullptr)
	{
		return equipmentTable->getHighlightedItems().size() > 0;
	}
	return false;
}

void Pawn::updateAnimations(float dt)
{
    auto pit = pendingFloatingTexts.begin();
    while (pit != pendingFloatingTexts.end()) {
        pit->delay -= dt;
        if (pit->delay <= 0.0f) {
            PendingFloatingText promoted = *pit;
            pit = pendingFloatingTexts.erase(pit);
            addFloatingText(promoted.text, promoted.color, promoted.duration);
        }
        else {
            ++pit;
        }
    }

    if (floatingTexts.empty()) return;

    for (auto& text : floatingTexts) {
        text->update(dt);
    }

    auto it = floatingTexts.begin();
    while (it != floatingTexts.end()) {
        if (!(*it)->isStillSeen()) {
            delete* it;
            it = floatingTexts.erase(it);
        }
        else {
            ++it;
        }
    }

    if (damageTintTimer > 0.0f) {
        damageTintTimer -= dt;
        if (damageTintTimer <= 0.0f && combinedSprite) {
            combinedSprite->setColor(sf::Color::White); // Reset to normal
        }
    }
}

bool Pawn::hasActiveAnimation() const
{
    return !floatingTexts.empty();
}

void Pawn::dropItems(EquipmentPile* pile)
{
    std::vector<Equipment*> highlightedItems = getHighlightedEquipment();

    for (Equipment* item : highlightedItems)
    {
        pile->addEquipment(item);
		removeEquipment(item);
    }
    equipmentTable->unhighlightAllItems();
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
            if (item->getFirstWordCapabilities() == "Shield")
            {
                types[0] = true;
            }
            else if (item->getFirstWordCapabilities() == "Helmet")
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

    addFloatingText("-" + std::to_string(value-rest), sf::Color(157, 163, 171));

    if (combinedSprite)
        combinedSprite->setColor(sf::Color(129, 142, 161));
    damageTintTimer = 0.5f;

    if (armour->getAttackValue() <= 0)
    {
        removeEquipment(armour);
        delete armour;
        if (armours[2] && type != "Covering")
        {
            Equipment* covering = findArmour("Covering");
            int rest2 = covering->reduceDurability(rest);

            addFloatingText("-" + std::to_string(rest - rest2), sf::Color(157, 163, 171));

            if (combinedSprite)
                combinedSprite->setColor(sf::Color(129, 142, 161));
            damageTintTimer = 0.5f;

            if (covering->getAttackValue() <= 0)
            {
                removeEquipment(covering);
                delete covering;
                if (rest2)
                    reduceHP(rest2);
            }
        }
        else
        {
            if (rest)
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
            if (item->getFirstWordCapabilities() == type)
            {
                return item;
            }
        }
    }
    throw std::runtime_error("Now such armour");
}

void Pawn::clearFlipIfNoLonger(const std::string& name)
{
    if (!flippedItems.count(name)) return;
    for (Equipment* e : equipment)
    {
        if (e->getName() == name) return;
    }
    flippedItems.erase(name);
}

void Pawn::setUpPosition()
{
    combinedSprite->setScale(scaleFactor, scaleFactor);
    combinedSprite->setPosition(xPos, yPos);
    combinedSprite->setOrigin(combinedSprite->getLocalBounds().width / 2, 
        combinedSprite->getLocalBounds().height / 2);
    combinedSprite->setRotation(rotationAngle);
}

void Pawn::calculateInitialActions()
{
    int peviousMaxActions = this->calculatedMaxActions;
	this->calculatedMaxActions = this->maxActions;
	for (Equipment* item : equipment)
	{
        if (item->getType() == "Armour" || (item->getType() == "Accesory" && 
                item->getName() != "dagger" && item->getName() != "long dagger"))
        {
            this->calculatedMaxActions -= item->getAttackActions();
        }
	}
    if (isMounted() && getSecondName() == "Horseman")
    {
        this->calculatedMaxActions += 1;
    }
	this->remainingActions -= (peviousMaxActions - this->calculatedMaxActions);
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
    {"one hand", 25},
	{"two hands", 26},
	{"one extra", 27},
	{"two extras", 28},
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
