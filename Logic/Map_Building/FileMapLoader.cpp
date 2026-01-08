#include "../../Headers/Map_Building/FileMapLoader.h"
#include "../../Headers/Map_Building/Map.h"

void FileMapLoader::setMapAdress(string adress)
{
    map_adress = adress;
}

void FileMapLoader::generateMap(Map &map)
{
    cout << "DEBUG: Incepem citirea hartii" << endl;

    char c;
    int i, j;

    ifstream file(map_adress);
    if (!file.is_open())
    {
        cerr << "Eroare la deschiderea fisierului: " << map_adress << endl;
        throw 1;
    }

    //dimensionam harta
    map.grid.clear();
    map.grid.resize(map.rows, vector<CELLTYPE>(map.cols, CELLTYPE::EMPTY_SPACE));

    for (i = 0; i < map.rows; i++)
    {
        for (j = 0; j < map.cols; j++)
        {
            file.get(c);

            //Check daca am terminat fisierul
            if (file.eof())
            {
                throw 4;
            }

            //Check pentru enter
            if (c == '\n' || c == '\r') {
                cerr << "Eroare: Linia " << i << " este mai scurta decat " << map.cols << endl;
                throw 3;
            }

            switch (c)
            {
                case '.':
                    map.grid[i][j] = CELLTYPE::ROAD;
                    break;
                case '#':
                    map.grid[i][j] = CELLTYPE::WALL;
                    break;

                case 'B':
                    map.grid[i][j] = CELLTYPE::BASE;
                    map.baza.x = i;
                    map.baza.y = j;
                    break;

                case 'S':
                    map.grid[i][j] = CELLTYPE::CHARGING_STATION;
                    map.chargingStationsLocations.push_back({i, j});
                    break;

                case 'D':
                    map.grid[i][j] = CELLTYPE::CLIENT;
                    map.clientsLocations.push_back({i, j});
                    break;

                case 'X':
                    map.grid[i][j] = CELLTYPE::EMPTY_SPACE;
                    break;

                default:
                    cerr << "Caracter necunoscut: " << (int)c << endl;
                    throw 2;
            }
        }

        file.get(c);

        // Aparent windows are fetish cu a pune \r\n. rezolvam aici
        if (c == '\r')
            {
            file.get(c); // Consumam '\n'
        }

        if (c != '\n' && !file.eof())
            {
            cerr << "Eroare: Linia " << i << " este mai LUNGA decat " << map.cols << endl;
            throw 3;
        }
    }

    map.displayMap();
    file.close();
    cout << endl << "DEBUG: Generare cu succes" << endl;
}

