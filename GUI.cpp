#include "GUI.h"

Gui::Gui(sf::RenderWindow* window)
	: window(window)
{
    initializeFont();
	grid = new Board(13, 19, 0.8f);
    pawns = new Pawns(grid, window);
    p1 = new Pawn("Mirmil", 1, 0, 5, 4, { 2,2 }, 7, 510.f, 504.0f);
    p2 = new Pawn("Lucjan", 2, 1, 5, 4, { 2,2 }, 7, 510.f, 504.0f);
    p3 = new Pawn("Zbyszko", 2, 1, 5, 4, { 2,2 }, 7, 510.f, 504.0f);
    e1 = new Equipment("sword", { 0,1 }, { 1, "hands" }, 1, 1, "Weapon", 5, "");
    e2 = new Equipment("helmet", { 0,1 }, { 1, "extras" }, 1, 1, "Armour", 5, "Helmet");
    e3 = new Equipment("plate armour", { 0,1 }, { 1, "extras" }, 2, 1, "Armour", 5, "Covering");
    e4 = new Equipment("shield", { 0,1 }, { 1, "hands" }, 2, 1, "Armour", 5, "Shield");
    e5 = new Equipment("bow", { 4,7 }, { 2, "hands" }, 1, 2, "Weapon", 5, "Ranged 2");
    e6 = new Equipment("crossbow", { 4,6 }, { 1, "hands" }, 2, 3, "Weapon", 4, "Ranged 2");
    e7 = new Equipment("war horse", { 0,1 }, { 1, "extras" }, 5, 1, "Weapon", 2, "Single use");
    e8 = new Equipment("two handed axe", { 0,2 }, { 2, "hands" }, 3, 4, "Weapon", 2, "");
}

Gui::~Gui()
{
    delete grid;
    delete pawns;
}

void Gui::start() {
    pawns->addPawn(p1, { -7, 5, 2 });
    pawns->addPawn(p2, { 9, -6, -3 });
    pawns->addPawn(p3, { 9, -5, -4 });
    pawns->addItemToPawn(0, e8, e7);
    pawns->addItemToPawn(1, e5, e2);
    pawns->addItemToPawn(2, e6, e4);
    while (window->isOpen())
    {
        window->clear(sf::Color(66, 82, 107));
        grid->drawBoard(*window);
        pawns->draw(isShiftKeyPressed);
        window->display();

        sf::Event event;
        while (window->pollEvent(event)) {
            keyPressed(event);
        }
    }
}

void Gui::keyPressed(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::LShift) {
        isShiftKeyPressed = true;
        pawns->handleShiftOn();
    }
    else if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::LShift) {
        isShiftKeyPressed = false;
        pawns->handleShiftOff();
    }
    else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2i mousePosition = sf::Mouse::getPosition(*window);
        pawns->handleClick(mousePosition);
    }
    else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right) {
        sf::Vector2i mousePosition = sf::Mouse::getPosition(*window);
        pawns->handleClickRight(mousePosition);
    }
    if (event.type == sf::Event::Closed)
    {
        window->close();
    }
}