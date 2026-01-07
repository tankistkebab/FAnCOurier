#include "../../Headers/Map_Building/FileMapLoader.h"
#include "../../Headers/Map_Building/Map.h"

void FileMapLoader::setMapAdress(string adress)
{
    map_adress = adress;
}

void FileMapLoader::generateMap(Map &map)
{
    //TODO
    char c;

    ifstream file;

    //DEschidem fisierul
    file.open(map_adress);
    if (!file.is_open())
    {
        cerr << "Eroare la deschiderea fisierului cu harta. Adresa fisierului este: " << map_adress << endl;
        throw 1;
    }

    //citim caracter cu caracter
    while (file.get(c))
    {

    }
}

