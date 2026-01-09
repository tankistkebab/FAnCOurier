#ifndef FANCOURIER_AGENT_H
#define FANCOURIER_AGENT_H

#pragma once
#include <vector>
#include <string>
#include <iostream>
#include "Map_Building/Map.h"

using namespace std;

enum class AgentType
{
    DRONE,
    ROBOT,
    SCOOTER
};

enum class MovementMode
{
    GROUND,
    AIR
};

enum class AgentState
{
    IDLE,
    MOVING,
    CHARGING,
    DEAD        // [cite: 47]
};

//struct pentru a nu avea o gramada de variabile imprastiate si a face codul mai lizibil
struct AgentStats
{
    char symbol;
    std::string name;
    MovementMode mode;
    int speed;          // pasi per tick
    int maxBattery;
    int batteryCons;    // consum per tick
    int costPerTick;    // cost de existenta
    int capacity;       // nr pachete
};



class Agent
{
protected:
    int id;
    POINT position;

    float currentBattery;
    AgentState state;

    AgentStats stats;

    const Map* map;

    vector<POINT> currentPath;

    void moveStep();
    void chargeStep();

public:
    Agent(int _id, POINT _startPos, const Map* _map);
    virtual ~Agent() = default;


    //getters
    bool isDead() const;
    bool isIdle() const;
    POINT getPosition() const;
    int getId() const;
    float getBattery() const;
    int getCapacity() const;
    int getCost() const;
    MovementMode getMovementMode() const;
    char getSymbol() const;

    //fiecare agent isi returneaza tipul
    virtual AgentType getType() const = 0;

    //loopul de apelat la inceput
    void update();

    void assignPath(const std::vector<POINT>& newPath);
};



#endif //FANCOURIER_AGENT_H