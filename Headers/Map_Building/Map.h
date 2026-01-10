#ifndef FANCOURIER_MAP_H
#define FANCOURIER_MAP_H
#pragma once
#include <iostream>
#include <vector>
#include <queue>
#include "../Utils.h"

class IMapGenerator;

using namespace std;

class Map
{
private:
    //nu le fac const, chiar daca ar trebui, pentru simplitatea codului
    int rows;
    int cols;

    //Harta (matrice) cu vector in loc de matrice clasica pentru felxibilitate si memory safety
    vector<vector<CELLTYPE>> grid;

    //Cache pentru statii, clienti si Baza (puncte de interes)
    vector<POINT> chargingStationsLocations;
    vector<POINT> clientsLocations;
    POINT baza;

public:
    Map();

    void setRows(int n);
    void setCols(int n);

    int getRows() const;
    int getCols() const;

    void displayMap();
    bool integrityCheck();

    //Pt ca in interiorul generatoarelor lucram f mult cu clasa harta, declaram generatoare ca friend class
    //Astefl putem accesa mult mai usor tot ce avem pe aici
    friend class FileMapLoader;
    friend class ProceduralMapGenerator;
    friend class Agent;
    friend class Pathfinder;
    friend class HiveMind;
    friend class SimManager;
};


#endif //FANCOURIER_MAP_H