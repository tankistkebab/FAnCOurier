#ifndef FANCOURIER_IMAPGENERATOR_H
#define FANCOURIER_IMAPGENERATOR_H
#include <fstream>
#include <sstream>
#include "Map.h"
#pragma once

using namespace std;

class Map;

// paradigma strategy
class IMapGenerator
{
public:
    virtual void generateMap(Map& map) = 0;

    virtual ~IMapGenerator() = default;
};


#endif //FANCOURIER_IMAPGENERATOR_H