#include "Armory.h"

Armory::Armory(sf::RenderWindow* window)
	:window(window)
{
    initializeFont();
	filename = "equipment";
	equipmentList = EquipmentManager::loadEquipmentFromJson(filename);
}

Armory::~Armory()
{
	for (Equipment* equipment : equipmentList) {
		delete equipment;
	}
}

void Armory::start() {
    while (window->isOpen())
    {
        window->clear(sf::Color(71, 31, 16));
        draw();
        window->display();

        /*sf::Event event;
        while (window->pollEvent(event)) {
            handleInput(event);
        }*/
        sf::Event event;
        while (window->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window->close();
            }
        }
    }
}


void Armory::exit()
{
}

void Armory::draw()
{
    sf::Text titleText("Armory", globalFont2, 50);
    titleText.setPosition(20, 10);
    titleText.setFillColor(sf::Color::White);
    window->draw(titleText);
    sf::Text text;
    text.setFont(globalFont2);
    text.setCharacterSize(50);
    text.setFillColor(sf::Color::White);

    sf::Vector2f position(20, 60); // starting position for drawing

    for (const Equipment* equipment : equipmentList) {
        text.setString(equipment->getName()); // Set equipment name here
        text.setPosition(position);
        window->draw(text);

        position.y += 40.f; // Move down for the next item
    }
}
