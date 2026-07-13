#include "WarriorPrep.h"

#include "ui/Layout.h"

WarriorPrep::WarriorPrep(Board* grid, Pawns* pawns)
	: grid(grid), pawns(pawns)
{
	fontSize = 40;
	blueText = initializeText("Blue choose your starting positions", &globalFont2,
		fontSize, sf::Color::White);
	redText = initializeText("Red choose your starting positions", &globalFont2,
		fontSize, sf::Color::White);
	backgroundSprite = loadBackgroundSprite(&backgroundTexture, "board");
	currentPlayer = 0;
	preperationDone = false;
}

class StartCoordinates {
public:
	static std::vector<std::tuple<int, int, int>> blueCoordinates;
	static std::vector<std::tuple<int, int, int>> redCoordinates;

	static std::vector<std::tuple<int, int, int>> getCoordinatesOf(int playerIndx)
	{
		if (playerIndx) {
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

void WarriorPrep::rebuildLayout(const sf::FloatRect& root, const sf::FloatRect& playArea)
{
	(void)playArea;
	Layout::scaleSpriteToCover(backgroundSprite, root);
	promptArea_ = Layout::uiBar(root, true, 120.f).toFloatRect();

	const sf::FloatRect blueBounds = blueText.getLocalBounds();
	blueText.setOrigin(blueBounds.width * 0.5f, 0.f);
	blueText.setPosition(promptArea_.left + promptArea_.width * 0.5f, promptArea_.top + 20.f);

	const sf::FloatRect redBounds = redText.getLocalBounds();
	redText.setOrigin(redBounds.width * 0.5f, 0.f);
	redText.setPosition(promptArea_.left + promptArea_.width * 0.5f, promptArea_.top + 20.f);
}

void WarriorPrep::handleEvent(const sf::Event& event, sf::Vector2f logicalMouse)
{
	if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
		handleClick(logicalMouse);
	}
}

void WarriorPrep::handleClick(sf::Vector2f logicalMouse)
{
	const std::vector<std::tuple<int, int, int>> startCoordinates =
		StartCoordinates::getCoordinatesOf(currentPlayer);
	for (const std::tuple<int, int, int>& coordinates : startCoordinates) {
		if (grid->hexDict[coordinates]->isClicked(logicalMouse)
			&& !grid->hexDict[coordinates]->isPawn()) {
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
	if (currentPlayer) {
		currentPawn->setRotationAngle(90.0f);
	}
	else {
		currentPawn->setRotationAngle(-90.0f);
	}
	currentPawn->setHexCoords(coordinates);
	currentPawn->setIsInGame(true);
	pawns->addPawn(currentPawn);
	if (pawnDict[currentPlayer].empty()) {
		if (currentPlayer == 1) {
			preperationDone = true;
		}
		else {
			currentPlayer = 1;
		}
	}
}

void WarriorPrep::addPawns(std::vector<Pawn*> inPawns, int playerIndx)
{
	pawnDict[playerIndx] = inPawns;
}

void WarriorPrep::draw(sf::RenderTarget& target)
{
	target.draw(backgroundSprite);
	grid->drawBoard(target);
	pawns->draw(false);
	drawText(target);
}

void WarriorPrep::drawText(sf::RenderTarget& target)
{
	target.draw(currentPlayer ? blueText : redText);
}
