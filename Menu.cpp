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

    float totalHeight = buttonLabels.size() * 70; // Total height of all buttons
    float startY = window->getSize().y / 2.0f - totalHeight / 2.0f;

    for (size_t i = 0; i < buttonLabels.size(); ++i) {
        sf::RectangleShape buttonRect(sf::Vector2f(250, 50)); // Adjust width and height as needed
        buttonRect.setPosition(window->getSize().x/2.0f - buttonRect.getSize().x / 2.0f, startY + i * 70);

        if (i == getSelectedItem()) {
            buttonRect.setFillColor(sf::Color(115, 115, 115)); // Highlight the selectedIndex button
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
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(*window);

        for (size_t i = 0; i < buttonLabels.size(); ++i) {
            sf::FloatRect buttonRect(window->getSize().x / 2.0f - 100, startY + i * 70, 200, 50); // Adjust position and size
            if (buttonRect.contains(mousePos.x, mousePos.y)) {
                selectedIndex = i;
                return;
            }
        }
    }
    else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(*window);

        for (size_t i = 0; i < buttonLabels.size(); ++i) {
            sf::FloatRect buttonRect(window->getSize().x / 2.0f - 100, startY + i * 70, 200, 50); // Adjust position and size
            if (buttonRect.contains(mousePos.x, mousePos.y)) {
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
