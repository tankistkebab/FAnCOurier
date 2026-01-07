#ifndef FANCOURIER_MAP_H
#define FANCOURIER_MAP_H
#pragma once
#include <iostream>
#include <vector>
#include "../Utils.h"

class IMapGenerator;

using namespace std;

class Map
{
private:
    int rows;
    int cols;

    //Harta (matrice) cu vector in loc de matrice clasica pentru felxibilitate si memory safety
    vector<vector<CELLTYPE>> grid;

    //Cache pentru statii, clienti si Baza (puncte de interes)
    vector<POINT> chargingStationsLocations;
    vector<POINT> clientsLocatioons;
    POINT baza;

public:

    void setRows(int n);
    void setCols(int n);

    int getRows();
    int getCols();

    friend class FileMapLoader;
    friend class ProceduralMapGenerator;
};


#endif //FANCOURIER_MAP_H