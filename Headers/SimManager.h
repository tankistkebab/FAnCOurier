#ifndef FANCOURIER_SIMMANAGER_H
#define FANCOURIER_SIMMANAGER_H
#pragma once
#include "Map_Building/IMapGenerator.h"
#include "Map_Building/FileMapLoader.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <memory>
#include <utility>

using namespace std;

class SimManager
{
private:
    string config_file_adress;
    int ticks, stations, clients, drones, robots, scooters, packages, spawn_frequency;
    Map* map; //pointer pentru a-l pasa mai usor
    //Nota: avand in vedere nr mic de vehicule, le stocam ca int-uri
    //pentru o solutie mai scalable am putea implementa stocare datelor in matrice de vectori de dim n x 2 (nume_variabilaa, valoare)

    //Uniqeu pointer pentru paradigma Strategy (evitam memory leak-uri)
    unique_ptr<IMapGenerator> map_generator;

    //Avem nevoi de 1 manager pt toata simularea, deci il facem singleton
    static SimManager* instance;
    SimManager();

public:
    //Stergem copy constructori pt a-l face singleton
    SimManager(const SimManager&) = delete;
    void operator==(const SimManager&) = delete;

    static SimManager* getInstance();

    //Incarcam in RAM parametrii
    void loadConfigData();
    void readDataFieldFromFile(ifstream* file, string check, int* val_to_set);    //Functie ajutatoare pentru incarcarea datelor

    //SimManager va fi si "Contextul" pentru strategy design pattern
    void setGenerator(unique_ptr<IMapGenerator> generator);


    void setConfigFileAdress(string adress);
    void displayData() const;
};


#endif //FANCOURIER_SIMMANAGER_H0