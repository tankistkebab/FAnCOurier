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
    packages_total = -1;
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

int SimManager::getClients()
{
    return clients;
}

int SimManager::getStations()
{
    return stations;
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
        readDataFieldFromFile(&file, check, &packages_total);
        cout << "DEBUG: packages a fost instantiat cu: " << packages_total << endl;

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
    cout << "Packages: " << packages_total << endl;
    cout << "Spawn_Frequency: " << spawn_frequency << endl;
}

void SimManager::setGenerator(unique_ptr<IMapGenerator> generator)
{
    map_generator = move(generator);
}

void SimManager::startGeneration() //Starts the map generation and checks if the map is valid.
{
    cout << "DEBUG: Incepem generarea" << endl;
    map_generator->generateMap(map);

    //Add integrity check to see if the map has a wall border
    if (!map.integrityCheck())
    {
        cerr << "Harta nu este valida! " << endl;
        throw 5;
    }
    cout << "DEBUG: Harta a fost validata cu succes!" << endl;
}

void SimManager::initFleet() {
    fleet.clear();

    int i, idCounter;
    idCounter = 1;

    for(i = 0; i < drones; ++i)
    {
        fleet.push_back(new Drone(idCounter++, map.baza, &map));
    }

    for(i = 0; i < robots; ++i)
    {
        fleet.push_back(new Robot(idCounter++, map.baza, &map));
    }

    for(i = 0; i < scooters; ++i)
    {
        fleet.push_back(new Scooter(idCounter++, map.baza, &map));
    }

    cout << "DEBUG: Flota initializata cu " << fleet.size() << " agenti." << endl;
}

void SimManager::spawnPackage()
{
    static int pkgIdCounter = 1;
    static mt19937 rng(random_device{}());

    if (map.clientsLocations.empty())
    {
        return;
    }
    //alegem un client random
    uniform_int_distribution<int> distClient(0, map.clientsLocations.size() - 1);
    POINT dest = map.clientsLocations[distClient(rng)];

    //generam random un reward si un deadline
    uniform_int_distribution<int> distReward(200, 800);
    uniform_int_distribution<int> distDeadline(10, 20);

    packages.emplace_back(pkgIdCounter++, dest, distReward(rng), currentTick, distDeadline(rng));

    cout << "DEBUG: Pachet generat (ID: " << pkgIdCounter-1 << ") catre Client." << endl;
}

void SimManager::generateFinalReport()
{
    int deliveredCount, totalRewards, missedCount, deadAgents;
    int penaltyDeadAgents, penaltyMissedPackages, totalPenalties;

    long totalOperatingCost, finalProfit;


    ofstream report("..//simulation.txt");

    if (!report.is_open())
    {
        cerr << "Eroare la crearea fisierului pentru raportul final" << endl;
        return;
    }

    deliveredCount = 0;
    totalRewards = 0;
    missedCount = 0;

    for (const auto& pkg : packages)
    {
        if (pkg.status == PACKAGESTATUS::DELIVERED)
        {
            deliveredCount++;
            totalRewards += pkg.reward;
        }
        else
        {
            missedCount++;
        }
    }

    //agenti si costuri de operatie
    deadAgents = 0;
    totalOperatingCost = 0;

    for (const auto* agent : fleet)
    {
        if (agent->isDead())
        {
            deadAgents++;
        }

        //Costul agentului * Durata simularii
        //Nota: Ar trebui sa tin cont cat timp a stat agentul mort, dar e prea tarziu sa mai schimb asta acm
        if (!agent->isDead())
        {
             totalOperatingCost += (long)agent->getCost() * currentTick;
        }
        else
        {
             totalOperatingCost += (long)agent->getCost() * currentTick;
        }
    }

    //penalizari
    penaltyDeadAgents = deadAgents * 500;
    penaltyMissedPackages = missedCount * 200;
    totalPenalties = penaltyDeadAgents + penaltyMissedPackages;

    finalProfit = totalRewards - totalOperatingCost - totalPenalties;

    report << "Raport simulare Hivemind! Nota: Anumite date nu sunt precise, ci se bazeaza pe estimari worst case scenario." << endl << endl;

    report << "Durata Simulare: " << currentTick << " / " << ticks << " ticks" << std::endl;
    report << "Total Agenti: " << fleet.size() << endl;
    report << "Total Pachete: " << packages.size() << endl;
    report << endl;

    report << "Pachete Livrate: " << deliveredCount << " (" << (packages.size() ? (deliveredCount * 100 / packages.size()) : 0) << "%)" << endl;
    report << "Pachete Ratate: " << missedCount << endl;
    report << "Agenti Pierduti: " << deadAgents << " (Dead Battery)" << endl;
    report << endl;

    report << "Venituri (Rewards): +" << totalRewards << endl;
    report << "Costuri Operare: -" << totalOperatingCost << endl;
    report << "Penalizari: -" << totalPenalties << endl;
    report << "Agenti morti: " << penaltyDeadAgents << ")" << endl;
    report << "Pachete ratate: " << penaltyMissedPackages << ")" << endl;
    report << "PROFIT NET FINAL: " << finalProfit << endl;
    report << endl;

    if (finalProfit > 0)
    {
        report << "REZULTAT: SUCCES (Profitabil)" << endl;
    }
    else
    {
        report << "REZULTAT: FALIMENT (Pierdere)" << endl;
    }

    report.close();
    cout << "Raport generat cu succes in 'simulation.txt'." << endl;
}

void SimManager::run()
{
    bool generationFinished, activePackagesExist;

    cout << "\n=== START SIMULARE HIVEMIND ===\n" << endl;

    initFleet();
    packages.clear();
    currentTick = 0;

    hiveMind = make_unique<HiveMind>(&map, fleet, packages);

    while (currentTick < ticks)
    {
        cout << endl << "TICK: " << currentTick + 1 << " / " << ticks <<  endl;

        //generam pachetele daca nu am atins limita
        if (currentTick % spawn_frequency == 0 && packages.size() < packages_total)
        {
            spawnPackage();
        }

        //update la hivemind si agenti
        hiveMind->update(currentTick);
        for (auto* agent : fleet) //din nou, fancy rau variabile de tip auto si foreach loopurile
        {
            agent->update();
        }

        //check daca am livrat toate pachetele
        if (packages.size() >= packages_total)
        {
            generationFinished = true;
        }
        else
        {
            generationFinished = false;
        }


        //check pentru pachetele in livrare
        activePackagesExist = false;
        for (const auto& pkg : packages)
        {
            if (pkg.status == PACKAGESTATUS::PENDING || pkg.status == PACKAGESTATUS::ASSIGNED)
            {
                activePackagesExist = true;
                cout << pkg.id << endl;
                break;
            }
        }

        cout << endl << "generationFinished = " << generationFinished << endl << "activePackegesExist = " << activePackagesExist << endl;

        if (generationFinished && !activePackagesExist)
        {
            cout << "Toate pachetele au fost livrate. Simularea se opreste anticipat la Tick:  " << currentTick + 1 << endl;
            break;
        }

        //avans timpul
        currentTick++;
    }

    cout << "Simularea s-a terminat" << endl;
    generateFinalReport();
}



