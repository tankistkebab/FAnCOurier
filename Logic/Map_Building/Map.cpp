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

bool Map::integrityCheck()
{
    int foundClients, foundStations;
    if (baza.x < 0 || baza.x >= rows || baza.y < 0 || baza.y >= cols)
    {
        cerr << "Eroare: Baza nu este definita corect!" << endl;
        return false;
    }

    //BFS
    queue<POINT> q;

    //Matrice de vizitare (defaul = false)
    vector<vector<bool>> visited(rows, vector<bool>(cols, false));

    foundClients = 0;
    foundStations = 0;

    //Start BFS
    q.push(baza);
    visited[baza.x][baza.y] = true;

    // VEctori de directie pentru a explora grid-ul
    int dx[] = {-1, 1, 0, 0};
    int dy[] = {0, 0, -1, 1};

    while (!q.empty())
    {
        POINT current = q.front();
        q.pop();

        if (grid[current.x][current.y] == CELLTYPE::CLIENT)
        {
            foundClients++;
        }
        else if (grid[current.x][current.y] == CELLTYPE::CHARGING_STATION)
        {
            foundStations++;
        }

        // Exploram vecinii
        for (int k = 0; k < 4; k++)
        {
            int nx, ny;

            nx = current.x + dx[k];
            ny = current.y + dy[k];

            // Cond de validare
            // da fie in interiorul grid-ului
            if (nx >= 0 && nx < rows && ny >= 0 && ny < cols)
            {
                // sa nu fie zid si sa nu fi fost deja vizitat
                if (grid[nx][ny] != CELLTYPE::WALL && !visited[nx][ny])
                {
                    visited[nx][ny] = true;
                    q.push({nx, ny});
                }
            }
        }
    }

    bool isOk = true;

    if (foundClients != clientsLocations.size())
    {
        cerr << " Harta invalida! Doar " << foundClients << " din " << clientsLocations.size() << " clienti sunt acces." << endl;
        isOk = false;
    }

    if (foundStations != chargingStationsLocations.size())
    {
        cerr << "CRITIC: Harta invalida! Doar " << foundStations << " din " << chargingStationsLocations.size() << " statii sunt acces" << endl;
        isOk = false;
    }

    return isOk;
}
