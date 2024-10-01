#include "GUI.h"

Gui::Gui(sf::RenderWindow* window)
	: window(window)
{
    initializeFont();
	grid = new Board(13, 19, 0.8f);
    pawns = new Pawns(grid, window);
    endTurnButton = Button(sf::Vector2f(window->getSize().x / 2 + 150,
        window->getSize().y - 70), sf::Vector2f(200, 50), "end turn");
    warriorPrep = new WarriorPrep(window, grid, pawns);
    backgroundSprite = loadBackgroundSprite(&backgroundTexture,"board");
    backgroundSprite.setPosition(0, 0);
}

Gui::~Gui()
{
    delete grid;
    delete pawns;
    delete warriorPrep;
}

void Gui::start() {
    warriorPrep->start();

    while (window->isOpen())
    {
        sf::Event event;
        while (window->pollEvent(event)) {
            keyPressed(event);
        }
    }
}

void Gui::keyPressed(const sf::Event& event) {
    bool changesOccurred = false;
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::LShift) {
        isShiftKeyPressed = true;
        pawns->handleShiftOn();
        changesOccurred = true;
    }
    else if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::LShift) {
        isShiftKeyPressed = false;
        pawns->handleShiftOff();
        changesOccurred = true;
    }
    else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2i mousePosition = sf::Mouse::getPosition(*window);
		if (!endTurnButton.click(mousePosition))
            pawns->handleClick(mousePosition);
        changesOccurred = true;
    }
    else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right) {
        sf::Vector2i mousePosition = sf::Mouse::getPosition(*window);
        pawns->handleClickRight(mousePosition);
        changesOccurred = true;
    }
    else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
        if (endTurnButton.unclick()) // checks if the button was clicked earlier
        {
            pawns->endTurn();
            changesOccurred = true;
        }
    }
    if (event.type == sf::Event::Closed)
    {
        window->close();
    }
    if (changesOccurred) display();
}

void Gui::addPawns(std::vector<Pawn*> pawns, int playerIndx)
{
    warriorPrep->addPawns(pawns, playerIndx);
}

void Gui::addWalls(int numWalls, int playerIndx)
{
	pawns->addWalls(numWalls, playerIndx);
}

void Gui::display()
{
    window->clear(sf::Color(66, 82, 107));
    window->draw(backgroundSprite);
    grid->drawBoard(*window);
    pawns->draw(isShiftKeyPressed);
	endTurnButton.draw(*window);
    window->display();
}
