#ifndef FANCOURIER_CUSTOMAGENTS_H
#define FANCOURIER_CUSTOMAGENTS_H

#pragma once
#include "Agent.h"

//cost mare, viteza mare, baterie mica, ignora zidurile
class Drone : public Agent
{
public:
    Drone(int id, POINT startPos, const Map* map) : Agent(id, startPos, map)
    {
        stats.name = "Drona";
        stats.symbol = 'A'; //
        stats.mode = MOVEMENTMODE::AIR; //
        stats.speed = 3;    //
        stats.maxBattery = 100;
        stats.batteryCons = 10;
        stats.costPerTick = 15;
        stats.capacity = 1;

        currentBattery = stats.maxBattery;
    }

    AGENTTYPE getType() const override;
};

//Robot, cost mic, baterie mare, viteza mica
class Robot : public Agent
{
public:
    Robot(int id, POINT startPos, const Map* map) : Agent(id, startPos, map)
    {
        stats.name = "Robot";
        stats.symbol = 'R';
        stats.mode = MOVEMENTMODE::GROUND;
        stats.speed = 1;
        stats.maxBattery = 300;
        stats.batteryCons = 2;
        stats.costPerTick = 1;
        stats.capacity = 4;

        currentBattery = stats.maxBattery;
    }

    AGENTTYPE getType() const override;
};

//scooter,cam cel  mai echilibrat
class Scooter : public Agent
{
public:
    Scooter(int id, POINT startPos, const Map* map) : Agent(id, startPos, map)
    {
        stats.name = "Scuter";
        stats.symbol = 'S'; //
        stats.mode = MOVEMENTMODE::GROUND;
        stats.speed = 2;    //
        stats.maxBattery = 200;
        stats.batteryCons = 5;
        stats.costPerTick = 4;
        stats.capacity = 2;

        currentBattery = stats.maxBattery;
    }

    AGENTTYPE getType() const override;
};


#endif //FANCOURIER_CUSTOMAGENTS_H