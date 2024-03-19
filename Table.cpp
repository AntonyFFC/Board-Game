#include "Table.h"

Table::Table()
{
}

Table::~Table()
{
}

void Table::draw()
{
}

bool Table::tableClicked(sf::Vector2i mousePosition)
{
    int clickMinY = minY + cellHeight;
    if (mousePosition.x >= minX && mousePosition.x <= maxX && mousePosition.y >= clickMinY && mousePosition.y <= maxY)
    {
        return true;
    }
    return false;
}

void Table::createTexture()
{
}

void Table::drawHeaders()
{
}

void Table::setUpDimensions()
{
    minX = position.x;
    minY = position.y;
    maxX = position.x+sumOfCellWidths;
    maxY = minY + cellHeight * numberOfItems;
}
