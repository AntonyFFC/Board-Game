#include "GUI.h"

Gui::Gui(const int screenWidth, const int screenHeight)
	: screenWidth(screenWidth), screenHeight(screenHeight)
{
    initializeFont();
	window = new sf::RenderWindow(sf::VideoMode(screenWidth, screenHeight), "Hex Board");
	grid = new Board(13, 19, 0.8f);
    pawns = new Pawns(grid);
    p1 = new Pawn("Mirmil", 1, 0, 5, 4, { 2,1 }, 7, 510.f, 504.0f);
    p2 = new Pawn("Lucjan", 2, 1, 5, 4, { 2,1 }, 7, 510.f, 504.0f);
    e1 = new Equipment("sword", 1, { 1, "hands" }, 1, 1, "Weapon", 5, "");
    e2 = new Equipment("helmet", 1, { 1, "extras" }, 1, 1, "Armour", 5, "");
    e3 = new Equipment("plate armour", 1, { 1, "hands" }, 1, 1, "Weapon", 5, "");
}

Gui::~Gui()
{
	delete window;
    delete grid;
    delete pawns;
}

void Gui::start() {
    pawns->addPawn(p1, { -7, 5, 2 });
    pawns->addPawn(p2, { 9, -6, -3 });
    pawns->addItemToPawn(0, e1, e2, e3);
    while (window->isOpen())
    {

        window->clear();
        grid->drawBoard(*window, isShiftKeyPressed);
        pawns->drawTurn(*window);
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