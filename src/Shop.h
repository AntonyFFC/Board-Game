#pragma once

#include "app/GameLaunchData.h"
#include "Button.h"
#include "Globals.h"
#include "IconWall.h"
#include "ShopCards.h"
#include "ShopPawns.h"
#include "ShopStorage.h"

#include <SFML/Graphics.hpp>
#include <string>

class Shop {
public:
    Shop();
    ~Shop();

    void prepare();
    void rebuildLayout(const sf::FloatRect& root);
    void handleEvent(const sf::Event& event, sf::Vector2f logicalMouse);
    void update(sf::Vector2f logicalMouse, sf::RenderWindow* cursorWindow);
    void draw(sf::RenderTarget& target, sf::Vector2f logicalMouse);

    bool wantsStartGame() const { return wantsStartGame_; }
    GameLaunchData collectLaunchData();

private:
    void nextTurn();
    bool buy(int cardNum);
    bool buyWall();
    void addCard(int cardNum);
    void reduceMoney(int price);
    void addLastItemToStorage();
    void drawChangeButton(sf::RenderTarget& target);
    void drawNextPlayerButton(sf::RenderTarget& target);
    void drawTurn(sf::RenderTarget& target);
    void drawBackground(sf::RenderTarget& target);
    void drawHeldItem(sf::RenderTarget& target, sf::Vector2f logicalMouse);
    void whatClicked(sf::Vector2f logicalMouse);
    void whatOffClicked(sf::Vector2f logicalMouse);
    void updateGoldText();
    void unClickAll();
    void beginGame();
    int viewedPlayerIndex() const;

    EquipmentCard* lastItem = nullptr;
    ShopCards* shopCards = nullptr;

    int currentRound = 1;
    int currentPlayerIndex = 0;
    int remainingGold = 6;
    int fontSize = 20;
    bool isPeeking = false;
    bool wantsStartGame_ = false;
    bool prepared_ = false;

    sf::Sprite backgroundSprite1;
    sf::Texture backgroundTexture1;
    sf::Sprite backgroundSprite2;
    sf::Texture backgroundTexture2;
    sf::Text titleText;
    sf::Text blueTurnText;
    sf::Text redTurnText;
    sf::Text goldText;
    sf::Text roundText;
    sf::Text heldItemHintText;
    IconWall wallIcon;
    Button changeButton;
    Button nextPlayer;
    Button peekButton;
    ShopPawns shopPawns[2];
    ShopStorage shopStorage[2];

    sf::FloatRect root_;
    sf::FloatRect bottomBar_;
};
