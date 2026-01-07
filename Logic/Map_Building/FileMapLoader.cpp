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
    for (i = 0; i < rows; i++)
    {
        for ( j = 0; i < cols; i++)
        {
            file.get(c);

            switch (c)
            {
                case '.': //Road
                    grid[i][j] = CELLTYPE::ROAD;
                    break;
                case '#':
                    //grid[i][j] = CE
                    break;

            }

        }
    }
}

