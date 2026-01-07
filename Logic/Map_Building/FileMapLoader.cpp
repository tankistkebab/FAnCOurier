#include "../../Headers/Map_Building/FileMapLoader.h"
#include "../../Headers/Map_Building/Map.h"

void FileMapLoader::setMapAdress(string adress)
{
    map_adress = adress;
}



void FileMapLoader::generateMap(Map &map) //Verifica inainte de funtie ca map exista
{
    //TODO
    int i, j;
    char c;

    ifstream file;

    i = 0;
    j = 0;

    //Deschidem fisierul
    file.open(map_adress);
    if (!file.is_open())
    {
        cerr << "Eroare la deschiderea fisierului cu harta. Adresa fisierului este: " << map_adress << endl;
        throw 1;
    }

    //citim caracter cu caracter
    for (i = 0; i < map.rows; i++)
    {
        for ( j = 0; i <= map.cols; i++)
        {
            file.get(c);

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
                    //salvam si coord bazei
                    map.baza.x = i;
                    map.baza.y = j;
                    break;

                case 'S':
                    map.grid[i][j] = CELLTYPE::CHARGING_STATION;
                    // salvam in buffer-ul de statii
                    map.chargingStationsLocations.push_back({i, j});
                    break;

                case 'D':
                    map.grid[i][j] = CELLTYPE::CLIENT;
                    // salvam in bufferul de clienti
                    map.clientsLocations.push_back({i, j});
                    break;

                case 'X':
                    map.grid[i][j] = CELLTYPE::EMPTY_SPACE;
                    break;

                case '/n':
                    //Verificam daca '/n' se afla unde trebuie
                    if (i != map.cols)
                    {
                        cerr << "Eroare la structura hartii" << endl;
                        throw 3;
                    }
                    break;

                case EOF:
                    cerr << "Final de fisier" << endl;
                    throw 3;

                default:
                    //eroare
                    cerr << "Caracter nerecunoscut in fisier: " << c << endl;
                    throw 2;
                    break;
            }
        }
    }
}

