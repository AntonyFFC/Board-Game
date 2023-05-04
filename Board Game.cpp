// SFML_test2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <SFML/Graphics.hpp>
#include <iostream>
#include "Board.h"

int main()
{
    const int screenWidth = 1300;
    const int screenHeight = 1000;
    sf::Color brown(165, 42, 42);
    sf::Color green(37, 142, 37);
    sf::Color picked(102, 255, 153);

    sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "Hex Board");
    //sf::ConvexShape hexTile(6);
    //hexTile.setPoint(0, sf::Vector2f(0, 0));
    //hexTile.setPoint(1, sf::Vector2f(50, 0));
    //hexTile.setPoint(2, sf::Vector2f(75, 43.3));
    //hexTile.setPoint(3, sf::Vector2f(50, 86.6));
    //hexTile.setPoint(4, sf::Vector2f(0, 86.6));
    //hexTile.setPoint(5, sf::Vector2f(-25, 43.3));
    //hexTile.setPosition(sf::Vector2f(100, 100));
    //hexTile.setOutlineColor(brown);
    //hexTile.setOutlineThickness(2.5f);
    //hexTile.setFillColor(green);

    //std::vector<sf::ConvexShape> hexagons;

    //// Create the grid of hexagons
    //const int numRows = 13;
    //const int numCols = 21;
    //const int numHexagons = numRows * numCols;
    //const float hexSize = 0.8f;
    //const float vertSpacing = 86.6f;
    //const float horizSpacing = 75.f;
    //float xOffset = 30.f;
    //float yOffset = 20.f;

    //for (int i = 0; i < numRows; i++) {
    //    for (int j = 0; j < numCols; j++) {
    //        sf::ConvexShape hex = hexTile;
    //        float x = j * horizSpacing*hexSize + xOffset;
    //        float y = i * vertSpacing* hexSize + yOffset + (j % 2) * vertSpacing* hexSize / 2;
    //        hex.setScale(hexSize, hexSize);
    //        hex.setPosition(x, y);

    //        hexagons.push_back(hex);
    //    }
    //}

    Board grid;

    while (window.isOpen())
    {

        window.clear();
        for (sf::ConvexShape& hexagon : grid.hexagons) {
            window.draw(hexagon);
        }
        window.display();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                for (sf::ConvexShape& hexagon : grid.hexagons) {
                    if (hexagon.getGlobalBounds().contains(mousePosition.x, mousePosition.y)) {
                        hexagon.setFillColor(picked);
                        hexagon.setOutlineColor(sf::Color::Yellow);
                    }
                }
            }
            if (event.type == sf::Event::Closed)
                window.close();
        }
    }
    return 0;
}

