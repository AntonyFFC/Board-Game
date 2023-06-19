#include "Globals.h"

sf::Font globalFont;

void initializeFont() {
    if (!globalFont.loadFromFile("assets/Modern.ttf")) {
        throw std::runtime_error("error while opening font file");
    }
}