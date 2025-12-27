#include "../../Headers/Map_Building/FileMapLoader.h"
#include "../../Headers/Map_Building/Map.h"

void FileMapLoader::setConfigFile(string adress)
{
    config_file_adress = adress;
}

void FileMapLoader::generateMap(Map& map)
{
    string line;

    config_file.open(config_file_adress);
    if (!config_file.is_open())
    {
        cout << "Eroare la deschiderea fisierului. Cod unic: 0" << endl;
        exit(EXIT_FAILURE);
    }

    //citim primul caracter invizibil (Byte Order Mark, \uFEFF pus la inceputul fisierelor UTF-8), mi-am invatat lectia
    //din lab-ul cu aplicatia de autobuze

    getline(config_file, line);

    stringstream ss(line);

    getline(ss, line, ' ');

    //Byte Order Mark, \uFEFF pus la inceputul fisierelor UTF-8. Nesimtitul nu voia sa apara vizual orice i-as fi facut
    line.erase(remove_if(line.begin(), line.end(),
                [](unsigned char c) { return !isdigit(c); }), line.end());
    cout << "DEBUG: Nr de linii citite: " << line << endl;
    map.rows = stoi(line);

    cout << "DEBUG: TEST " << line << endl;
    getline(ss, line);
    cout << "DEBUG: Nr de coloane citite: " << line << endl;
    map.cols = stoi(line);

    cout << "Finir" << endl;
}

//Trb refacut tot


