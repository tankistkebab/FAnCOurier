#ifndef FANCOURIER_PROCEDURALMAPGENERATOR_H
#define FANCOURIER_PROCEDURALMAPGENERATOR_H
#include <random>
#include <stack>
#include <algorithm>
#include "IMapGenerator.h"


class ProceduralMapGenerator : public IMapGenerator
{
private:
    int clients, charging_stations, randomness;
    string config_file_adress;

    mt19937 rng; //Generator modern, mai bun decat rand()

    void buildMap(Map &map, int startX, int startY);
    void addRandomRoads(Map &map);
    void placeObjects(Map &map);
public:
    ProceduralMapGenerator(int clients, int charging_stations, int randomness);

    void generateMap(Map &map) override;
};


#endif //FANCOURIER_PROCEDURALMAPGENERATOR_H