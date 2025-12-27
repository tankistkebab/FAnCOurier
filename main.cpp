#include <iostream>

#include "Headers/SimManager.h"

using namespace std;

int main()
{
    //Instantiem singletoanele (managerele)
    SimManager* sim_manager = SimManager::getInstance();

    cout << "Bine ati venit la aplicatia de simulare FAnCOurier! " << endl;
    cout << "Incepe initializarea aplicatiei." << endl;

    //Incarcam parametrii de configurare + verificam
    string config_file_adress = "..//Database//simulation_setup.txt";
    sim_manager->setConfigFileAdress(config_file_adress);
    sim_manager->loadConfigData();
    sim_manager->displayData();



    return 0;
}