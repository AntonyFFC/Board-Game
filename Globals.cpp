#include "Globals.h"

sf::Font globalFont;

void initializeFont() {
    if (!globalFont.loadFromFile("assets/Arial Unicode MS.ttf")) {
        throw std::runtime_error("error while opening font file");
    }
}