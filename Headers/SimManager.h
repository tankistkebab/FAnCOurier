#ifndef FANCOURIER_SIMMANAGER_H
#define FANCOURIER_SIMMANAGER_H
#pragma once
#include "Map_Building/IMapGenerator.h"
#include "Map_Building/FileMapLoader.h"
#include "Agent.h"
#include "CustomAgents.h"
#include "Package.h"
#include "HiveMind.h"
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <memory>
#include <random>

using namespace std;

class SimManager
{
private:
    string config_file_adress;
    int ticks, stations, clients, packages_total, drones, robots, scooters, spawn_frequency;

    int currentTick = 0;

    Map map;

    vector<Agent*> fleet;
    vector<Package> packages;
    unique_ptr<HiveMind> hiveMind;

    unique_ptr<IMapGenerator> map_generator;

    // Singleton stuff
    static SimManager* instance;
    SimManager();



    void initFleet();
    void spawnPackage();

public:
    // Singleton stuff
    SimManager(const SimManager&) = delete;
    void operator=(const SimManager&) = delete;

    static SimManager* getInstance();

    // Getteri
    int getClients();
    int getStations();

    // Config
    void setConfigFileAdress(string adress);
    void loadConfigData();
    void readDataFieldFromFile(ifstream* file, string check, int* val_to_set);
    void displayData() const;

    void setGenerator(unique_ptr<IMapGenerator> generator);
    void startGeneration();

    void run();

    // Raport final
    void generateFinalReport();
};

#endif //FANCOURIER_SIMMANAGER_H