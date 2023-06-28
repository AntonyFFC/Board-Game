#include "Globals.h"

sf::Font globalFont;
sf::Font globalFont2;

void initializeFont() {
    if (!globalFont.loadFromFile("assets/Arial Unicode MS.ttf")) {
        throw std::runtime_error("error while opening font file");
    }
    if (!globalFont2.loadFromFile("assets/Modern.ttf")) {
        throw std::runtime_error("error while opening font file");
    }
}