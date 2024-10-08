#include "WarriorPrep.h"

WarriorPrep::WarriorPrep(sf::RenderWindow* window, Board* grid, Pawns* pawns)
	: window(window), grid(grid), pawns(pawns)
{
	fontSize = 40;
	blueText = initializeText("Blue choose your starting positions", &globalFont2, 
		fontSize, sf::Color::White);
	blueText.setPosition(window->getSize().x/2 - 200, window->getSize().y / 3);
	redText = initializeText("Red choose your starting positions", &globalFont2,
		fontSize, sf::Color::White);
	redText.setPosition(window->getSize().x / 2 - 200, window->getSize().y / 3);
	backgroundSprite = loadBackgroundSprite(&backgroundTexture, "board");
	backgroundSprite.setPosition(0, 0);
	currentPlayer = 0;
	preperationDone = false;
}

WarriorPrep::~WarriorPrep()
{
}

class StartCoordinates {
public:
	static std::vector<std::tuple<int, int, int>> blueCoordinates;
	static std::vector<std::tuple<int, int, int>> redCoordinates;

	static std::vector<std::tuple<int, int, int>> getBlueCoordinates() {
		return blueCoordinates;
	}

	static std::vector<std::tuple<int, int, int>> getRedCoordinates() {
		return redCoordinates;
	}
	
	static std::vector<std::tuple<int, int, int>> getCoordinatesOf(int playerIndx)
	{
		if (playerIndx)
		{
			return blueCoordinates;
		}
		return redCoordinates;
	}
};

std::vector<std::tuple<int, int, int>> StartCoordinates::blueCoordinates = {
	{-8, 2, 6}, {-7, 2, 5}, {-7, 3, 4}, {-7, 4, 3}, {-7, 5, 2}, {-8, 6, 2},
	{-8, 5, 3}, {-8, 4, 4}, {-8, 3, 5}
};
std::vector<std::tuple<int, int, int>> StartCoordinates::redCoordinates = {
	{10, -7, -3}, {9, -6, -3}, {9, -5, -4}, {9, -4, -5}, {9, -3, -6}, {10, -3, -7},
	{10, -4, -6}, {10, -5, -5}, {10, -6, -4}
};

void WarriorPrep::start()
{
	display();

	while (window->isOpen() && !preperationDone)
	{
		sf::Event event;
		while (window->pollEvent(event)) {
			eventHandler(event);
		}
	}
}
void WarriorPrep::eventHandler(sf::Event event)
{
	if (event.type == sf::Event::Closed) {
		window->close();
	}
	else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
		sf::Vector2i mousePosition = sf::Mouse::getPosition(*window);
		handleClick(mousePosition);
		display();
	}
}

void WarriorPrep::handleClick(sf::Vector2i mousePosition)
{
	std::vector<std::tuple<int, int, int>> startCoordinates = StartCoordinates::getCoordinatesOf(currentPlayer);
	for (std::tuple<int, int, int> coordinates : startCoordinates)
	{
		if (grid->hexDict[coordinates]->isClicked(mousePosition))
		{
			putPawn(coordinates);
			break;
		}
	}
}

void WarriorPrep::putPawn(std::tuple<int, int, int> coordinates)
{
	Pawn* currentPawn = pawnDict[currentPlayer].back();
	pawnDict[currentPlayer].pop_back();
	currentPawn->scale(0.05f);
	if (currentPlayer)
		currentPawn->setRotationAngle(90.0f);
	else
		currentPawn->setRotationAngle(-90.0f);
	currentPawn->setHexCoords(coordinates);

	currentPawn->setIsInGame(true);
	pawns->addPawn(currentPawn);
	if (pawnDict[currentPlayer].empty())
	{
		if (currentPlayer == 1)
		{
			preperationDone = true;
		}
		else
		{
			currentPlayer = 1;
		}
	}
}

void WarriorPrep::addPawns(std::vector<Pawn*> pawns, int playerIndx)
{
	pawnDict[playerIndx] = pawns;
}

void WarriorPrep::display()
{
	window->clear(sf::Color(66, 82, 107));
	window->draw(backgroundSprite);
	grid->drawBoard(*window);
	pawns->draw(false);
	drawText();
	window->display();
}

void WarriorPrep::drawText()
{
	if (currentPlayer)
	{
		window->draw(blueText);
		return;
	}
	window->draw(redText);
}

