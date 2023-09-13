#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "Pawn.h"
#include "Equipment.h"
class Card
{
	Card();
	~Card();

	void draw(sf::RenderTexture& window);
	void draw(sf::RenderWindow& window);

	bool isClicked(sf::Vector2i mousePosition);
private:
	sf::Vector2f position;
	sf::Sprite sprite;
	sf::Texture texture;
};

// add child clases with either equipment or pawns as attributes in innitial constructor values
