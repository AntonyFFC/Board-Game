#include "TableUtils.h"

void drawSpaceIcon(Equipment::SpaceOccupied space, sf::RenderWindow* target, std::map<std::string, sf::Sprite> &iconSprites)
{
    int added = 0;
    for (int i = 0; i < space.numSpaces; i++)
    {
        if (space.spaceType == "hands")
        {
            target->draw(iconSprites["hand-line-icon"]);
            iconSprites["hand-line-icon"].move(15, 0);
        }
        else
        {
            target->draw(iconSprites["plus-round-line-icon"]);
            iconSprites["plus-round-line-icon"].move(15, 0);
        }
        added++;
    }
    if (space.spaceType == "hands")
    {
        iconSprites["hand-line-icon"].move(-15 * added, 0);
    }
    else
    {
        iconSprites["plus-round-line-icon"].move(-15 * added, 0);
    }

}

void drawTypeIcon(std::string type, sf::RenderWindow* target, std::map<std::string, sf::Sprite>& iconSprites)
{
    if (type == "Weapon")
    {
        target->draw(iconSprites["crossed-swords-icon"]);
    }
    else if (type == "Armour")
    {
        target->draw(iconSprites["shield-icon"]);
    }
    else
    {
        target->draw(iconSprites["four-squares-icon"]);
    }
}

std::map<std::string, sf::Sprite> initializeSpriteMap(std::vector<sf::Texture*> &iconTextures)
{
    std::string folderPath = "assets/icons/";
    std::string searchPattern = folderPath + "*.png";
    WIN32_FIND_DATAA findData;
    HANDLE findHandle = FindFirstFileA(searchPattern.c_str(), &findData);

    std::map<std::string, sf::Sprite> sprites;

    if (findHandle != INVALID_HANDLE_VALUE)
    {
        do
        {
            std::string fileName = findData.cFileName;
            fileName = fileName.substr(0, fileName.length() - 4);
            std::string filePath = folderPath + fileName + ".png";

            // Load the texture
            sf::Texture* texture;
            texture = new sf::Texture;
            if (!texture->loadFromFile(filePath))
            {
                delete texture;
                continue;
            }
            // This holds the textures so that the sprites work
            iconTextures.push_back(texture);
            sf::Sprite sprite(*texture);
            sprites[fileName] = sprite;
        } while (FindNextFileA(findHandle, &findData));
        FindClose(findHandle);
    }
    else
    {
        throw std::runtime_error("Invalid handle value");
    }
    return sprites;
}

std::vector<std::function<std::string(const Equipment&)>> initializeFunctions()
{
    std::vector<std::function<std::string(const Equipment&)>> newFunctions;
    newFunctions.push_back([](const Equipment& item) { return item.getName(); });
    newFunctions.push_back([](const Equipment& item) { return item.rangeToString(); });
    newFunctions.push_back([](const Equipment& item) { return item.spaceToString(); }); //here draw the sprite with space
    newFunctions.push_back([](const Equipment& item) { return std::to_string(item.getAttackValue()); });
    newFunctions.push_back([](const Equipment& item) { return std::to_string(item.getAttackActions()); });
    newFunctions.push_back([](const Equipment& item) { return item.getType(); }); //here draw the sprite with type
    newFunctions.push_back([](const Equipment& item) { return std::to_string(item.getPrice()); });
    newFunctions.push_back([](const Equipment& item) { return item.getAdditionalCapabilities(); });
    return newFunctions;
}

sf::RectangleShape initializeCells()
{
    sf::RectangleShape newCell;
    newCell.setFillColor(sf::Color::Transparent);
    newCell.setOutlineColor(sf::Color::White);
    newCell.setOutlineThickness(1.f);
    return newCell;
}