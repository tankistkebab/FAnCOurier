#include "../Headers/SimManager.h"

#include <ranges>


//pentru singleton, instance si constructorul trebuie declarate explicit si in .cpp
SimManager* SimManager::instance = nullptr;

SimManager::SimManager()
{
    //initializam cu nr negative pentru a putea descoperi erori in cod mai usor
    ticks = -1;
    stations = -1;
    clients = -1;
    drones = -1;
    robots = -1;
    scooters = -1;
    packages = -1;
    spawn_frequency = -1;

    map_generator = nullptr;
}

SimManager* SimManager::getInstance()
{
    if (instance == nullptr)
    {
        instance = new SimManager();
    }
    return instance;
}

void SimManager::setConfigFileAdress(string adress)
{
    config_file_adress = adress;
    cout << "DEBUG: config_file_adress a fost instantiat cu: " << adress << endl;
}

void SimManager::readDataFieldFromFile(ifstream *file, string check, int* val_to_set)
{
    string line, value;

    getline(*file, line);

    stringstream ss(line);

    getline(ss, value, ' ');
    if (value != check)
    {
        cerr << "Eroare la citirea " << check << ". Verifica fromatarea fisierului de configurare" << endl;
        exit(EXIT_FAILURE);
    }

    getline(ss, value);
    *val_to_set = stoi(value);
}

void SimManager::loadConfigData()
{
    string line, value;
    ifstream file;

    file.open(config_file_adress);
    if (!file.is_open())
    {
        cerr << "Eroare la deschiderea fisierului de configurare" << endl;
        exit(EXIT_FAILURE);
    }

    //Incepem citirea fisierului
    getline(file, line);

    //Prima linie ne indica tipul fisierului
    if (line == "Plaintext")
    {//Facem si cateva verificari pentru fisier
        //M asi N. Citire manuala pentru ca are un format special
        getline(file, line);
        stringstream ss(line);

        getline(ss, value, ' ');
        if (value != "MAP_SIZE:")
        {
            cerr << "Eroare la citirea MAP_SIZE. Verifica fromatarea fisierului de configurare." << endl;
            exit(EXIT_FAILURE);
        }

        //Nu mai facem variabile M si N, citim si setam direct Rows si Cols in map
        getline(ss, value, ' ');
        map.setRows(stoi(value));
        cout << "DEBUG: Rows a fost instantiat cu: " << map.getRows() << endl;

        getline(ss, value);
        map.setCols(stoi(value));
        cout << "DEBUG: Cols a fost instantiat cu: " << map.getCols() << endl;

        //Restul de fielduri sunt standard, deci putem folosi o functie
        //Am ales sa tin DEBUG-urile aici, in cazul in care apar erori cu pasarea prin referinta
        string check; //Utilizat pentru a verifica formatul fiecarui field citit

        //ticks
        check = "MAX_TICKS:";
        readDataFieldFromFile(&file, check, &ticks);
        cout << "DEBUG: tick a fost instatiat cu: " << ticks << endl;

        //stations
        check = "MAX_STATIONS:";
        readDataFieldFromFile(&file, check, &stations);
        cout << "DEBUG: stations a fost instantiat cu: " << stations << endl;

        //clients
        check = "CLIENTS_COUNT:";
        readDataFieldFromFile(&file, check, &clients);
        cout << "DEBUG: clients a fost instantiat cu: " << clients << endl;

        //Linie libera pentru a indica ca am terminat cu datele de configurare si trecem la datele despre agenti
        //o citim, verificam sa fie goala si o ignoram;
        getline(file, line);
        if (!line.empty())
        {
            cout << "ATENTIE! Fisierul contine elemente pe liniile speratoare: " << line << endl;
        }

        //Citim flota de start
        //dornes;
        check = "DRONES:";
        readDataFieldFromFile(&file, check, &drones);
        cout << "DEBUG: drones a fost instantiat cu: " << drones << endl;

        //robots
        check = "ROBOTS:";
        readDataFieldFromFile(&file, check, &robots);
        cout << "DEBUG: robots a fost instantiat cu: " << robots << endl;

        //scooters
        check = "SCOOTERS:";
        readDataFieldFromFile(&file, check, &scooters);
        cout << "DEBUG: scooters a fost instantiat cu: " << scooters << endl;

        //Linie libera pentru a indica ca am terminat cu datele flotei de start si trecem la dinamica
        //o citim, verificam sa fie goala si o ignoram;
        getline(file, line);
        if (!line.empty())
        {
            cout << "ATENTIE! Fisierul contine elemente pe liniile speratoare: " << line << endl;
        }

        //Citim datele de dinamica
        //packages
        check = "TOTAL_PACKAGES:";
        readDataFieldFromFile(&file, check, &packages);
        cout << "DEBUG: packages a fost instantiat cu: " << packages << endl;

        //spawn frequency
        check = "SPAWN_FREQUENCY:";
        readDataFieldFromFile(&file, check, &spawn_frequency);
        cout << "DEBUG: spawn_frequency a fost instantiat cu: " << spawn_frequency << endl;

        cout << "DEBUG: Instantiere cu succes!" << endl;
    }
    else
    {
        cout << "Tip fisier nerecunoscut. Verifica structura fisierului: " << config_file_adress << endl;
        file.close();
        exit(EXIT_FAILURE);
    }

    file.close();
}

void SimManager::displayData() const
{
    cout << endl << "========== Displaying the data stored in SimManager ==========" << endl;
    cout << "Map.rows: " << map.getRows() << endl;
    cout << "Map.cols: " << map.getCols() << endl;
    cout << "Ticks: " << ticks << endl;
    cout << "Stations: " << stations << endl;
    cout << "Clients: " << clients << endl;
    cout << "Drones: " << drones << endl;
    cout << "Robots: " << robots << endl;
    cout << "Scooters: " << scooters << endl;
    cout << "Packkages: " << packages << endl;
    cout << "Spawn_Frequency: " << spawn_frequency << endl;
}

void SimManager::setGenerator(unique_ptr<IMapGenerator> generator)
{
    map_generator = move(generator);
}

void SimManager::startGeneration()
{
    map_generator->generateMap(map);
}



