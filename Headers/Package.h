#ifndef FANCOURIER_PACKAGE_H
#define FANCOURIER_PACKAGE_H
#pragma once
#include "Utils.h"

enum class PACKAGESTATUS
{
    PENDING,
    ASSIGNED,
    DELIVERED,
    EXPIRED
};

struct Package
{
    int id;
    POINT destination; //Coord clientului
    int reward;        //200 - 800
    int assignedAgentId; // -1 daca nu e dat unui curier

    //tick-uri
    int creationTime;  // tick-ul creatiei
    int deadlineTicks; //tickuri pentru livrare

    PACKAGESTATUS status;

    Package(int _id, POINT _dest, int _reward, int _time, int _deadline): id(_id), destination(_dest), reward(_reward), creationTime(_time), deadlineTicks(_deadline), status(PACKAGESTATUS::PENDING), assignedAgentId(-1) {}
};



#endif //FANCOURIER_PACKAGE_H