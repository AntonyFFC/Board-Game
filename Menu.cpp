#include "Menu.h"

Menu::Menu(const int screenWidth, const int screenHeight) 
    : screenWidth(screenWidth), screenHeight(screenHeight) {
    initializeFont();
    window = new sf::RenderWindow(sf::VideoMode(screenWidth, screenHeight), "Skirmish");
    selectedIndex = 0;
    buttonLabels = {
    "Local", "Multiplayer",
    "Player", "Armory", "Settings", "Exit"
    };
    totalHeight = buttonLabels.size() * 70; // Total height of all buttons
    startY = window->getSize().y / 2.0f - totalHeight / 2.0f;
    interface1 = new Gui(window);
    armory1 = new Armory(window);
    backgroundSprite = loadBackground();
    backgroundSprite.setPosition(0, 0);
    initializeButtons();
}

Menu::~Menu()
{
    delete interface1;
    delete armory1;
    delete window;
}

void Menu::start() {
    while (window->isOpen())
    {
        window->clear(sf::Color(71, 31, 16));
        window->draw(backgroundSprite);
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

    for (size_t i = 0; i < buttons.size(); i++) {

        if (i == getSelectedItem()) {
            buttons[i].setBackgroundColor(sf::Color(115, 115, 115)); // Highlight the selectedIndex button
        }
        else {
            buttons[i].setBackgroundColor(sf::Color::Transparent);
        }
        
        buttons[i].draw(*window);
    }
}

void Menu::handleInput(sf::Event event) {
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(*window);

        for (size_t i = 0; i < buttons.size(); i++) {
            if (buttons[i].isClicked(mousePos)) {
                selectedIndex = i;
                return;
            }
        }
    }
    else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(*window);

        for (size_t i = 0; i < buttonLabels.size(); ++i) {
            if (buttons[i].isClicked(mousePos)) {
                selectedIndex = i;
                callSelected(getSelectedItem());
                return;
            }
        }
    }
    else if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Up) {
            selectedIndex = (selectedIndex + buttonLabels.size() - 1) % buttonLabels.size();
        }
        else if (event.key.code == sf::Keyboard::Down) {
            selectedIndex = (selectedIndex + 1) % buttonLabels.size();
        }
        else if (event.key.code == sf::Keyboard::Enter) {
            // Depending on the selectedIndex item, perform an action
            callSelected(getSelectedItem());
        }
    }
    else if (event.type == sf::Event::Closed)
    {
        window->close();
    }
}

int Menu::getSelectedItem() {
    // Return the index of the selectedIndex menu item
    // This can be based on which button is currently highlighted or selectedIndex
    return selectedIndex;
}

void Menu::callSelected(int selected)
{
    switch (selected) {
    case 0: // 2 Player Game Button
        interface1->start();
    case 1: // Multiplayer Game Button
        // Start a multiplayer game
        break;
    case 2: // Player Button
        // Go to the player settings
        break;
    case 3: // Armory Button
        armory1->start();
    case 4: // Settings Button
        // Open the settings menu
        break;
    case 5:
        window->close();
    }
}

sf::Sprite Menu::loadBackground()
{
    sf::Sprite newSprite;
    if (!backgroundTexture.loadFromFile("assets/backgrounds/main.png")) {
        throw std::runtime_error("Unable to load background");
    }

    newSprite = sf::Sprite(backgroundTexture);
    return newSprite;
}

void Menu::initializeButtons()
{
    for (int i =0; i <buttonLabels.size();i++)
    {
        sf::Vector2f position(window->getSize().x / 2.0f - 125, startY + i * 70);
        sf::Vector2f dimensions(250, 50);
        buttons.push_back(Button(position, dimensions, buttonLabels[i]));
    }
}
