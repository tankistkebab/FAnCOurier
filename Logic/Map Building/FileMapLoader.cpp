#include "../../Headers/Map Building/FileMapLoader.h"

void FileMapLoader::setFileAdress(string adresa)
{
    file.open(adresa);
    if (!file.is_open())
    {
        cerr << "Eroare la deschiderea fisierului pentru citirea hartii" << endl;
        exit(EXIT_FAILURE);
    }
}

void FileMapLoader::generateMap(Map &map)
{

}

