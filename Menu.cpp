#include "Menu.h"

Menu::Menu(const int screenWidth, const int screenHeight) 
    : screenWidth(screenWidth), screenHeight(screenHeight) {
    initializeFont();
    window = new sf::RenderWindow(sf::VideoMode(screenWidth, screenHeight), "Menu");
    selected = 0;
    buttonLabels = {
    "Local", "Multiplayer",
    "Player", "Armory", "Settings"
    };
}

Menu::~Menu()
{
    delete window;
}

void Menu::start() {
    while (window->isOpen())
    {
        window->clear(sf::Color(66, 82, 107));
        draw();
        window->display();

        sf::Event event;
        while (window->pollEvent(event)) {
            handleInput(event);
        }
    }
}

void Menu::draw() {
    sf::Text titleText("Skirmish", globalFont2, 50);
    titleText.setPosition(window->getSize().x / 2.0f - titleText.getGlobalBounds().width / 2.0f, 50);
    titleText.setFillColor(sf::Color::White);
    window->draw(titleText);

    float totalHeight = buttonLabels.size() * 70; // Total height of all buttons
    float startY = window->getSize().y / 2.0f - totalHeight / 2.0f;

    for (size_t i = 0; i < buttonLabels.size(); ++i) {
        sf::RectangleShape buttonRect(sf::Vector2f(250, 50)); // Adjust width and height as needed
        buttonRect.setPosition(window->getSize().x/2.0f - buttonRect.getSize().x / 2.0f, startY + i * 70);

        if (i == getSelectedItem()) {
            buttonRect.setFillColor(sf::Color::Green); // Highlight the selected button
        }
        else {
            buttonRect.setFillColor(sf::Color::Blue);
        }

        buttonRect.setOutlineThickness(2); // Border thickness
        buttonRect.setOutlineColor(sf::Color::White);

        sf::Text buttonText(buttonLabels[i], globalFont2, 30);
        buttonText.setPosition(buttonRect.getPosition().x + buttonRect.getSize().x / 2.0f - buttonText.getGlobalBounds().width / 2.0f,
            buttonRect.getPosition().y + buttonRect.getSize().y / 2.0f - buttonText.getGlobalBounds().height / 2.0f);

        window->draw(buttonRect);
        window->draw(buttonText);
    }
}

void Menu::handleInput(sf::Event event) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Up) {
            selected--;
            if (selected < 0)
                selected = 0;
        }
        else if (event.key.code == sf::Keyboard::Down) {
            selected++;
            if (selected > buttonLabels.size()-1)
                selected = buttonLabels.size()-1;
        }
        else if (event.key.code == sf::Keyboard::Enter) {
            // Depending on the selected item, perform an action
            switch (getSelectedItem()) {
            case 0: // 2 Player Game Button
                interface1.start();
            case 1: // Multiplayer Game Button
                // Start a multiplayer game
                break;
            case 2: // Player Button
                // Go to the player settings
                break;
            case 3: // Armory Button
                // Open the armory or equipment screen
                break;
            case 4: // Settings Button
                // Open the settings menu
                break;
            }
        }
    }
    else if (event.type == sf::Event::Closed)
    {
        window->close();
    }
}

int Menu::getSelectedItem() {
    // Return the index of the selected menu item
    // This can be based on which button is currently highlighted or selected
    return selected;
}