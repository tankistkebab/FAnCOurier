#ifndef FANCOURIER_SIMMANAGER_H
#define FANCOURIER_SIMMANAGER_H
#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

class SimManager
{
private:
    string config_file_adress;
    int M, N, ticks, stations, clients, drones, robots, scooters, packages, spawn_frequency;
    //Nota: avand in vedere nr mic de vehicule, le stocam ca int-uri
    //pentru o solutie mai scalable am putea implementa stocare in matrici de vectori de dim n x 2

    //Avem nevoi de 1 manager pt toata simularea, deci il facem singleton
    static SimManager* instance;
    SimManager();

public:
    //Stergem copy constructori pt al face singleton
    SimManager(const SimManager&) = delete;
    void operator==(const SimManager&) = delete;

    static SimManager* getInstance();

    void loadConfigData();

    //setters
    void readDataFieldFromFile(ifstream* file, string check, int* val_to_set);
    void setConfigFileAdress(string adress);
    void displayData();
};


#endif //FANCOURIER_SIMMANAGER_H0