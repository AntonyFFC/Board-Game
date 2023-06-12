#include "GUI.h"

Gui::Gui(const int screenWidth, const int screenHeight)
	: screenWidth(screenWidth), screenHeight(screenHeight)
{
	window = new sf::RenderWindow(sf::VideoMode(screenWidth, screenHeight), "Hex Board");
	grid = new Board(13, 19, 0.8f);
    p1 = new Pawn("Mirmi³", 1, 0, 5, 4, { 2,1 }, 7, 510.f, 504.0f);
    e1 = new Equipment("sword", 1, { 1, "hands" }, 1, 1, "Weapon", 5, "");
    e2 = new Equipment("helmet", 1, { 1, "extras" }, 1, 1, "Armour", 5, "");
    e3 = new Equipment("shield", 1, { 1, "hands" }, 1, 1, "Weapon", 5, "");
}

Gui::~Gui()
{
	delete window;
    delete grid;
}

void Gui::start() {
    grid->hexDict[{-7, 5, 2}]->setPawn(true, p1);
    std::cout << grid->hexDict[{-7, 5, 2}]->pawn->addEquipment(e1);
    std::cout << grid->hexDict[{-7, 5, 2}]->pawn->addEquipment(e2);
    std::cout << grid->hexDict[{-7, 5, 2}]->pawn->addEquipment(e3);
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
            {
                window->close();
            }
        }
    }
}