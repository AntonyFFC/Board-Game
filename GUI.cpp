#include "GUI.h"

Gui::Gui(const int screenWidth, const int screenHeight)
	: screenWidth(screenWidth), screenHeight(screenHeight)
{
	window = new sf::RenderWindow(sf::VideoMode(screenWidth, screenHeight), "Hex Board");
	sM = selectionManager();
	grid = Board(13, 19, 0.8f);
    Red red;
    std::string Name = "Mirmi³";
    p1 = Pawn(Name, 1, &red, 5, 4, 4, 7);
    

}

Gui::~Gui()
{
	delete window;
}

void Gui::start() {
    
    while (window->isOpen())
    {

        window->clear();
        for (auto& pair : grid.hexDict) {
            Hex& hexagon = pair.second;
            window->draw(hexagon.shape);
            /*window.draw(hexagon.rockShape);*/
        }
        window->draw(p1.getImage());
        window->display();

        sf::Event event;
        while (window->pollEvent(event)) {
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePosition = sf::Mouse::getPosition(*window);
                sM.handleClick(grid, mousePosition);
            }
            if (event.type == sf::Event::Closed)
                window->close();
        }
    }
}