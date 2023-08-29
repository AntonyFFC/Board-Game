#include "Armory.h"

template <typename T, size_t N>
int getSumOfArray(T(&arr)[N]) {
    int sum = 0;
    for (size_t i = 0; i < N; ++i) {
        sum += arr[i];
    }
    return sum;
}

Armory::Armory(sf::RenderWindow* window)
	:window(window), headers{ "Name","left-right-arrow-icon","circle-line-icon","bomb-blast-icon",
 "history-icon","cube-icon","dollar-icon","Other" }, cellWidths{ 100,60,60,50,50,50,50,500 }
{
    tableRenderTexture.create(window->getSize().x, window->getSize().y);
    fontSize = 20;
    position = sf::Vector2f(20, 60);
    initializeFont();
	filename = "equipment";
	equipmentList = EquipmentManager::loadEquipmentFromJson(filename);
    initializeText();
    iconSprites = initializeSpriteMap(iconTextures);
    functions = initializeFunctions();
    cell = initializeCells();
    cellHeight = 30;
    sumOfCellWidths = getSumOfArray(cellWidths);
}

Armory::~Armory()
{
	for (Equipment* equipment : equipmentList) {
		delete equipment;
	}
}

void Armory::start() {
    tableRenderTexture.clear(sf::Color::Transparent);
    createTexture();
    tableRenderTexture.display();
    sf::Sprite tableSprite(tableRenderTexture.getTexture());

    while (window->isOpen())
    {
        window->clear(sf::Color(71, 31, 16));
        window->draw(tableSprite);
        window->display();

        /*sf::Event event;
        while (window->pollEvent(event)) {
            handleInput(event);
        }*/
        sf::Event event;
        while (window->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window->close();
            }
        }
    }
}


void Armory::exit()
{
}

void Armory::createTexture()
{
    tableRenderTexture.draw(titleText);
    cell.setPosition(position);
    setPosSpriteMap(position.x, position.y, iconSprites);
    setScalSpriteMap(0.04, iconSprites);
    text.setPosition(position.x, position.y);
    cell.setFillColor(sf::Color(156, 84, 84));

    for (int i = 0; i < 8; i++)
    {
        cell.setSize(sf::Vector2f(cellWidths[i], cellHeight));
        tableRenderTexture.draw(cell);
        if (headers[i].substr(headers[i].length() - 4) != "icon")
        {
            text.setString(headers[i]);
            tableRenderTexture.draw(text);
        }
        else
        {
            tableRenderTexture.draw(iconSprites[headers[i]]);
        }
        cell.move(cellWidths[i], 0);
        text.move(cellWidths[i], 0);
        moveSpriteMap(cellWidths[i], 0, iconSprites);
    }
    cell.setFillColor(sf::Color(200, 200, 200));
    cell.move(-sumOfCellWidths, cellHeight);
    text.move(-sumOfCellWidths, cellHeight);
    moveSpriteMap(-sumOfCellWidths, cellHeight, iconSprites);

    for (Equipment* item : equipmentList)
    {
        for (int i = 0; i < 8; i++)
        {
            cell.setSize(sf::Vector2f(cellWidths[i], cellHeight));
            tableRenderTexture.draw(cell);
            if (i == 2)
            {
                drawSpaceIcon(item->getSpaceOccupied(), tableRenderTexture, iconSprites);
            }
            else if (i == 5)
            {
                drawTypeIcon(item->getType(), tableRenderTexture, iconSprites);
            }
            else
            {
                text.setString(functions[i](*static_cast<const Equipment*>(item)));
                tableRenderTexture.draw(text);
            }
            cell.move(cellWidths[i], 0);
            text.move(cellWidths[i], 0);
            moveSpriteMap(cellWidths[i], 0, iconSprites);
        }
        cell.move(-sumOfCellWidths, cellHeight);
        text.move(-sumOfCellWidths, cellHeight);
        moveSpriteMap(-sumOfCellWidths, cellHeight, iconSprites);
    }
}

void Armory::initializeText()
{
    sf::Text newText;
    newText.setFont(globalFont2);
    newText.setCharacterSize(fontSize);
    newText.setFillColor(sf::Color::White);
    text = newText;

    sf::Text newTitleText("Armory", globalFont2, fontSize*1.5);
    newTitleText.setPosition(20, 10);
    newTitleText.setFillColor(sf::Color::White);
    titleText = newTitleText;
}
