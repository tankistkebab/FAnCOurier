#ifndef FANCOURIER_HIVEMIND_H
#define FANCOURIER_HIVEMIND_H
#pragma once
#include <vector>
#include <iostream>
#include <algorithm>
#include <map>
#include "Agent.h"
#include "Package.h"
#include "Map_Building/Map.h"
#include "Pathfinder.h"

using namespace std;

struct TaskBid
{
    int agentIndex;
    int packageIndex;
    double score; //profitul estimat

    //sortare descrescatoare
    bool operator>(const TaskBid& other) const
    {
        return score > other.score;
    }
};

class HiveMind
{
private:
    int currentTick;

    Map* map;
    vector<Agent*>& fleet;       //ref la flota stocata in SimManager
    vector<Package>& packages;   //ref la pachetele din SimManager


    void checkBatteryLevels();

    void assignTasks();

    void checkDeliveries();

    void checkDeadlines(int currentTick);

    POINT findNearestChargingStation(POINT currentPos, MOVEMENTMODE mode);
    int ManhattanDist(POINT a, POINT b);

public:
    HiveMind(Map* _map, std::vector<Agent*>& _fleet, std::vector<Package>& _packages): map(_map), fleet(_fleet), packages(_packages) {}

    // Metoda apelata la fiecare tick
    void update(int tick);
};




#endif //FANCOURIER_HIVEMIND_H