#include "../../Headers/Map_Building/Map.h"

void Map::setRows(const int n)
{
    rows = n;
}

void Map::setCols(const int n)
{
    cols = n;
}

int Map::getRows() const
{
    return rows;
}

int Map::getCols() const
{
    return cols;
}

Map::Map()
{
    rows = -1;
    cols = -1;

    baza.x = -1;
    baza.y = -1;
}
