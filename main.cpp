#include <cstdlib>
#include <iostream>
#include <limits>
#include <memory>

#include "Headers/SimManager.h"
#include "Headers/Map_Building/ProceduralMapGenerator.h"

using namespace std;

// Reads an integer from stdin, re-prompting on non-numeric input instead of
// leaving cin in a failed state (which would otherwise loop forever).
// Exits cleanly if stdin is closed.
static int readInt(int minValue, int maxValue)
{
    int value;
    while (true)
    {
        if (cin >> value)
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if (value >= minValue && value <= maxValue)
            {
                return value;
            }
            cout << "Valoarea trebuie sa fie intre " << minValue << " si " << maxValue << ". Incearca din nou:" << endl;
            continue;
        }

        if (cin.eof())
        {
            cout << endl << "Input inchis. La revedere!" << endl;
            exit(EXIT_SUCCESS);
        }

        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Input invalid, introdu un numar intreg:" << endl;
    }
}

int main()
{
    int selector, status;

    string config_file_adress = "..//Database//simulation_setup.txt";

    selector = 0;
    status = 1;

    //Instantiem singletoanele (managerele)
    SimManager* sim_manager = SimManager::getInstance();

    cout << "Bine ati venit la aplicatia de simulare FAnCOurier! " << endl;
    cout << "Incepe initializarea aplicatiei." << endl;

    //Incarcam parametrii de configurare + verificam
    sim_manager->setConfigFileAdress(config_file_adress);
    sim_manager->loadConfigData();
    sim_manager->displayData();


    cout << endl <<  "Initializare completa. Bine ati venit!" << endl;
    while (status)
    {
        cout << "Va rog selectati actiunea pe care doriti sa o realizati:" << endl;
        cout << "1. Incarcare harta din fisier" << endl;
        cout << "2. Generare procedurala" << endl;
        cout << "0. Iesire" << endl;

        selector = readInt(numeric_limits<int>::min(), numeric_limits<int>::max());
        switch (selector)
        {
            //Incarcare din fisier
            case 1:
                try
                {
                    //TODO
                    //string adresa = "Database//map.txt"; //optional setam adresa hartii
                    sim_manager->setGenerator(make_unique<FileMapLoader>());
                    sim_manager->startGeneration();

                    sim_manager->run();
                    status = 0;
                    break;
                }
                catch (int n)
                {
                    //Error handling
                    switch (n)
                    {
                        case 1:
                            cerr << "Eroare la initializarea FileMapLoader. Incearca din nou" << endl;
                            break;
                        case 2:
                            cerr << "Harta contine un caracter necunoscut" << endl;
                            break;
                        case 3:
                            cerr << "Eroare la structura hartii" << endl;
                            break;
                        case 4:
                            cerr << "Fisierul s-a terminat prea devreme" << endl;
                            break;
                        case 5:
                            cerr << "Harta generata nu este buna! " << endl;
                            break;
                        default:
                            cout << "Eroare necunoscuta." << endl;
                            exit(EXIT_FAILURE);
                    }
                }
                break;
            case 2:
                try
                {
                    int procent;
                    cout << "Care este procentul de drumuri pe care il doriti (0-100)?" << endl;
                    procent = readInt(0, 100);

                    sim_manager->setGenerator(make_unique<ProceduralMapGenerator>(sim_manager->getClients(), sim_manager->getStations(), procent));
                    sim_manager->startGeneration();

                    sim_manager->run();
                    status = 0;
                    break;
                }
                catch (int n)
                {
                    switch (n)
                    {
                        case 1:
                            cerr << "Procentul introdus nu este valid. Trebuie sa fie o valoarea intre 0 si 100." << endl;
                            break;
                        case 2:
                            cerr << "Harta e prea mica pentru a plasa atatea obiecte! Verifica fisierul de initializare. " << endl;
                            exit(EXIT_FAILURE);
                        case 3:
                            cerr << "Harta nu este valida. Incearca din nou" << endl;
                            break;
                        case 5:
                            cerr << "Harta generata nu este buna! " << endl;
                            break;
                        default:
                            cerr << "Eroare necunoscuta." << endl;
                            exit(EXIT_FAILURE);
                    }
                }
                break;
            case 0:
                status = 0;
                break;
            default:
                cout << "Optiune necunoscuta. Incearca din nou!" << endl;
                break;
        }
    }
    return 0;
}