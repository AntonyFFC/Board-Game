#include "Shop.h"

#include "SpriteUtils.h"
#include "ui/Layout.h"

namespace {
    constexpr float kBottomBarHeight = 90.f;
    constexpr float kButtonWidth = 200.f;
    constexpr float kButtonHeight = 50.f;
    constexpr float kButtonSpacing = 50.f;
}

Shop::Shop()
    : changeButton({}, { kButtonWidth, kButtonHeight }, "-}")
    , nextPlayer({}, { kButtonWidth, kButtonHeight }, "next player")
    , peekButton({}, { kButtonWidth, kButtonHeight }, "peek opponent")
{
    initializeFont();
    titleText = initializeText("Shop", &globalFont2, fontSize * 1.5, sf::Color::White);
    blueTurnText = initializeText("Blue", &globalFont2, fontSize * 1.5, sf::Color::Blue);
    redTurnText = initializeText("Red", &globalFont2, fontSize * 1.5, sf::Color::Red);
    goldText = initializeText("Gold: 6", &globalFont2, fontSize * 1.5, sf::Color(222, 164, 38));
    roundText = initializeText("Round: 1", &globalFont2, fontSize * 1.5, sf::Color::White);
    heldItemHintText = initializeText("Click a pawn to equip, or storage to keep",
        &globalFont2, fontSize, sf::Color(244, 228, 196, 210));
    backgroundSprite1 = loadBackgroundSprite(&backgroundTexture1, "shop1");
    backgroundSprite2 = loadBackgroundSprite(&backgroundTexture2, "shop2");
    shopCards = new ShopCards();
}

Shop::~Shop()
{
    delete shopCards;
}

void Shop::prepare()
{
    if (prepared_) {
        return;
    }
    shopCards->prepareDecks(root_);
    prepared_ = true;
}

void Shop::rebuildLayout(const sf::FloatRect& root)
{
    root_ = root;
    bottomBar_ = Layout::uiBar(root, false, kBottomBarHeight).toFloatRect();

    Layout::scaleSpriteToCover(backgroundSprite1, root);
    Layout::scaleSpriteToCover(backgroundSprite2, root);

    const float barCenterY = bottomBar_.top + bottomBar_.height * 0.5f;
    const float totalWidth = kButtonWidth * 3.f + kButtonSpacing * 2.f;
    const float startX = root.left + (root.width - totalWidth) * 0.5f;

    peekButton.setPosition({ startX, barCenterY - kButtonHeight * 0.5f });
    changeButton.setPosition({ startX + kButtonWidth + kButtonSpacing, barCenterY - kButtonHeight * 0.5f });
    nextPlayer.setPosition({ startX + (kButtonWidth + kButtonSpacing) * 2.f, barCenterY - kButtonHeight * 0.5f });

    titleText.setPosition(root.left + root.width * 0.02f, root.top + root.height * 0.02f);
    blueTurnText.setPosition(root.left + root.width * 0.02f, root.top + root.height * 0.07f);
    redTurnText.setPosition(root.left + root.width * 0.02f, root.top + root.height * 0.07f);
    goldText.setPosition(root.left + root.width * 0.88f, root.top + root.height * 0.02f);
    roundText.setPosition(root.left + root.width * 0.78f, root.top + root.height * 0.02f);
    wallIcon.setPosition(root.left + root.width * 0.02f, root.top + root.height * 0.45f);

    shopCards->setPositions(root_);
    shopStorage[0].setPanel(Layout::fromNormalized(root, 0.82f, 0.05f, 0.16f, 0.88f).toFloatRect());
    shopStorage[1].setPanel(Layout::fromNormalized(root, 0.82f, 0.05f, 0.16f, 0.88f).toFloatRect());
    shopPawns[0].setColumn(Layout::fromNormalized(root, 0.58f, 0.10f, 0.22f, 0.75f).toFloatRect());
    shopPawns[1].setColumn(Layout::fromNormalized(root, 0.58f, 0.10f, 0.22f, 0.75f).toFloatRect());

    if (prepared_) {
        shopCards->fillEmptySlots(root_);
    }
}

void Shop::handleEvent(const sf::Event& event, sf::Vector2f logicalMouse)
{
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        whatClicked(logicalMouse);
    }
    else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
        unClickAll();
        whatOffClicked(logicalMouse);
    }
    else if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Right || event.key.code == sf::Keyboard::Left) {
            shopCards->flipPage();
        }
    }
}

void Shop::update(sf::Vector2f logicalMouse, sf::RenderWindow* cursorWindow)
{
    const bool buttonHovered = Button::updateAll(logicalMouse, nullptr);
    const bool cardHovered = shopCards->updateHover(logicalMouse, !isPeeking);
    const bool pawnHovered = shopPawns[viewedPlayerIndex()].updateHover(logicalMouse);
    Button::applyCursor(cursorWindow, buttonHovered || cardHovered || pawnHovered);
}

void Shop::draw(sf::RenderTarget& target, sf::Vector2f logicalMouse)
{
    drawBackground(target);
    target.draw(titleText);
    drawTurn(target);
    target.draw(goldText);
    target.draw(roundText);
    shopCards->draw(&target);
    shopStorage[viewedPlayerIndex()].draw(&target);
    shopPawns[viewedPlayerIndex()].draw(&target);
    drawChangeButton(target);
    drawNextPlayerButton(target);
    peekButton.setText(isPeeking ? "back to my shop" : "peek opponent");
    peekButton.draw(dynamic_cast<sf::RenderWindow&>(target));
    wallIcon.draw(dynamic_cast<sf::RenderWindow&>(target));
    if (!isPeeking) {
        drawHeldItem(target, logicalMouse);
    }
}

GameLaunchData Shop::collectLaunchData()
{
    GameLaunchData data;
    data.pawns[0] = shopPawns[0].getPawns();
    data.pawns[1] = shopPawns[1].getPawns();
    data.walls[0] = shopStorage[0].getNumberOfWalls();
    data.walls[1] = shopStorage[1].getNumberOfWalls();
    wantsStartGame_ = false;
    return data;
}

bool Shop::buy(int cardNum)
{
    const int price = shopCards->getPriceOfCard(cardNum);
    if (remainingGold < price) {
        return false;
    }

    addLastItemToStorage();
    addCard(cardNum);
    if (lastItem != nullptr) {
        lastItem->click(false);
    }
    shopCards->removeCard(cardNum);
    reduceMoney(price);
    updateGoldText();
    return true;
}

bool Shop::buyWall()
{
    if (remainingGold < 1) {
        return false;
    }
    addLastItemToStorage();
    wallIcon.setIsBeingClicked(false);
    shopStorage[currentPlayerIndex].addWall();
    reduceMoney(1);
    updateGoldText();
    return true;
}

void Shop::addCard(int cardNum)
{
    if (shopCards->currentPage) {
        lastItem = shopCards->getCard<EquipmentCard>(cardNum);
    }
    else {
        WarriorCard* warriorCard = shopCards->getCard<WarriorCard>(cardNum);
        warriorCard->getWarrior()->setSide(currentPlayerIndex);
        shopPawns[currentPlayerIndex].addPawn(warriorCard->getWarrior());
    }
}

void Shop::reduceMoney(int price)
{
    remainingGold -= price;
}

void Shop::addLastItemToStorage()
{
    if (lastItem != nullptr) {
        shopStorage[currentPlayerIndex].addCard(lastItem);
        lastItem = nullptr;
    }
}

void Shop::nextTurn()
{
    addLastItemToStorage();
    remainingGold = 6;
    currentPlayerIndex = (currentPlayerIndex + 1) % 2;
    if (!currentPlayerIndex) {
        currentRound++;
        if (currentRound == 3) {
            beginGame();
            return;
        }
    }
    roundText.setString("Round: " + std::to_string(currentRound));
    shopCards->fillEmptySlots(root_);
    updateGoldText();
}

void Shop::drawHeldItem(sf::RenderTarget& target, sf::Vector2f logicalMouse)
{
    if (lastItem == nullptr) {
        return;
    }

    lastItem->drawHeldPreview(target, logicalMouse);

    const sf::FloatRect hintBounds = heldItemHintText.getLocalBounds();
    heldItemHintText.setPosition(
        root_.left + root_.width * 0.5f - (hintBounds.width * 0.5f + hintBounds.left),
        bottomBar_.top - 30.f);
    target.draw(heldItemHintText);
}

void Shop::drawChangeButton(sf::RenderTarget& target)
{
    changeButton.setText(shopCards->currentPage ? "{-" : "-}");
    changeButton.draw(dynamic_cast<sf::RenderWindow&>(target));
}

void Shop::drawNextPlayerButton(sf::RenderTarget& target)
{
    if (remainingGold == 0) {
        nextPlayer.draw(dynamic_cast<sf::RenderWindow&>(target));
    }
}

void Shop::drawTurn(sf::RenderTarget& target)
{
    target.draw(viewedPlayerIndex() ? blueTurnText : redTurnText);
}

void Shop::drawBackground(sf::RenderTarget& target)
{
    target.draw(shopCards->currentPage ? backgroundSprite2 : backgroundSprite1);
}

void Shop::whatClicked(sf::Vector2f logicalMouse)
{
    const int cardNum = shopCards->whichCardClicked(logicalMouse);
    const int pawnNum = shopPawns[viewedPlayerIndex()].whichPawnClicked(logicalMouse);
    const int storageCardNum = shopStorage[viewedPlayerIndex()].whichItemClicked(logicalMouse);

    if (peekButton.isClicked(logicalMouse)) {
        isPeeking = !isPeeking;
        shopPawns[0].hideAllTables();
        shopPawns[1].hideAllTables();
    }
    else if (changeButton.isClicked(logicalMouse)) {
        shopCards->flipPage();
    }
    else if (cardNum != -1 && !isPeeking) {
        shopCards->clickCard(cardNum);
    }
    else if (nextPlayer.isClicked(logicalMouse) && !remainingGold && !isPeeking) {
        nextTurn();
    }
    else if (wallIcon.isClicked(logicalMouse) && !isPeeking) {
        wallIcon.setIsBeingClicked(true);
    }
    else if (pawnNum != -1) {
        if (lastItem == nullptr || isPeeking) {
            shopPawns[viewedPlayerIndex()].togglePawnEquipmentTable(pawnNum);
        }
        else {
            if (!shopPawns[currentPlayerIndex].addEquipmentToPawn(pawnNum, lastItem->getItem())) {
                shopStorage[currentPlayerIndex].addCard(lastItem);
            }
            lastItem = nullptr;
        }
    }
    else if (storageCardNum != -1 && !isPeeking) {
        lastItem = shopStorage[currentPlayerIndex].takeItem(storageCardNum);
    }
    else if (shopStorage[currentPlayerIndex].isClicked(logicalMouse) && lastItem != nullptr && !isPeeking) {
        shopStorage[currentPlayerIndex].addCard(lastItem);
        lastItem = nullptr;
    }
}

void Shop::whatOffClicked(sf::Vector2f logicalMouse)
{
    if (isPeeking) {
        return;
    }

    const int cardNum = shopCards->whichCardClicked(logicalMouse);
    if (cardNum != -1) {
        buy(cardNum);
    }
    else if (wallIcon.isClicked(logicalMouse)) {
        buyWall();
    }
}

void Shop::updateGoldText()
{
    goldText.setString("Gold: " + std::to_string(remainingGold));
}

void Shop::unClickAll()
{
    wallIcon.setIsBeingClicked(false);
    shopCards->unClickAll();
}

int Shop::viewedPlayerIndex() const
{
    return isPeeking ? (currentPlayerIndex + 1) % 2 : currentPlayerIndex;
}

void Shop::beginGame()
{
    shopPawns[0].hideAllTables();
    shopPawns[1].hideAllTables();
    wantsStartGame_ = true;
}
