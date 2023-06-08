#include "GUI.h"

Gui::Gui(const int screenWidth, const int screenHeight)
	: screenWidth(screenWidth), screenHeight(screenHeight)
{
	window = new sf::RenderWindow(sf::VideoMode(screenWidth, screenHeight), "Hex Board");
	grid = new Board(13, 19, 0.8f);
    grid->hexDict[{0,0,0}]->setPawn(true, new Pawn("Mirmi³", 1, 0, 5, 4, { 2,1 }, 7, 510.f, 504.0f));
}

Gui::~Gui()
{
	delete window;
}

void Gui::start() {
    
    while (window->isOpen())
    {

        window->clear();
        grid->drawBoard(*window);
        window->display();

        sf::Event event;
        while (window->pollEvent(event)) {
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePosition = sf::Mouse::getPosition(*window);
                grid->handleClick(mousePosition);
            }
            if (event.type == sf::Event::Closed)
                window->close();
        }
    }
}