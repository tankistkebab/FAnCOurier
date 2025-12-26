#ifndef FANCOURIER_IMAPGENERATOR_H
#define FANCOURIER_IMAPGENERATOR_H
#include <iostream>
#include <fstream>
#pragma once

using namespace std;

class Map;

// paradigma strategy
class IMapGenerator
{
public:
    virtual void generateMap(Map& map) = 0;
    virtual void setConfigFile(string adress) = 0;

    virtual ~IMapGenerator() = default;
};


#endif //FANCOURIER_IMAPGENERATOR_H