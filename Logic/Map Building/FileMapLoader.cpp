#include "../../Headers/Map Building/FileMapLoader.h"

void FileMapLoader::setConfigFile(string adress)
{
    config_file_adress = adress;
}

void FileMapLoader::generateMap(Map &map)
{
    char ch;

    config_file.open(config_file_adress);
    if (!config_file.is_open())
    {
        cout << "Eroare la deschiderea fisierului. Cod unic: 0" << endl;
        exit(EXIT_FAILURE);
    }

    while (config_file.get(ch))
    {

    }
}



