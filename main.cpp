#include <filesystem>
#include <iostream>
#include <memory>

#include "Headers/SimManager.h"

using namespace std;

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

    while (status)
    {
        cout << endl <<  "Initializare completa. Bine ati venit!" << endl;
        cout << "Va rog selectati actiunea pe care doriti sa o realizati:" << endl;
        cout << "1. Incarcare harta din fisier" << endl;

        cin >> selector;
        switch (selector)
        {
            //Incarcare din fisier
            case 1:
                try
                {
                    //TODO
                    //string adresa = "Database//map.txt"; //optional setam adresa hartii
                    sim_manager->setGenerator(make_unique<FileMapLoader>());
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

                        default:
                            cout << "Eroare necunoscuta." << endl;
                            exit(EXIT_FAILURE);
                            break;
                    }
                }
            default:
                cout << "Optiune necunoscuta. Incearca din nou!" << endl;
                break;
        }
    }
    return 0;
}