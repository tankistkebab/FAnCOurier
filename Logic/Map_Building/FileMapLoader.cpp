#include "../../Headers/Map_Building/FileMapLoader.h"
#include "../../Headers/Map_Building/Map.h"

void FileMapLoader::setMapAdress(string adress)
{
    map_adress = adress;
}

void FileMapLoader::generateMap(Map &map)
{
    char c;
    ifstream file;
    file.open(map_adress);
    if (!file.is_open())
    {
        cerr << "Eroare la deschiderea fisierului cu harta. Adresa este: " << map_adress << endl;
        throw 1;
    }

    while (file.get(c))
    {

    }
}

