#include "../../Headers/Map_Building/Map.h"

Map::Map()
{
    rows = -1;
    cols = -1;

    baza.x = -1;
    baza.y = -1;
}

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

void Map::displayMap()
{
    int i, j;

    cout << endl;
    for (i =  0; i < rows; i++)
    {
        for (j = 0; j < cols; j++)
        {
            switch (grid[i][j])
            {
                case CELLTYPE::ROAD:
                    cout << ".";
                    break;
                case CELLTYPE::WALL:
                    cout << "#";
                    break;
                case CELLTYPE::BASE:
                    cout << "B";
                    break;
                case CELLTYPE::CHARGING_STATION:
                    cout << "S";
                    break;
                case CELLTYPE::CLIENT:
                    cout << "D";
                    break;
                case CELLTYPE::EMPTY_SPACE:
                    cout << "X";
                    break;
                default:
                    cerr << "EROARE CRITICA: Harta contine caractere ilegale." << endl;
                    exit(EXIT_FAILURE);
            }
        }
        cout << endl;
    }
}
