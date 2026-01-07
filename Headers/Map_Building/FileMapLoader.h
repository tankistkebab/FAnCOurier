#ifndef FANCOURIER_FILEMAPLOADER_H
#define FANCOURIER_FILEMAPLOADER_H
#include "IMapGenerator.h"
#include <algorithm>

class FileMapLoader : public IMapGenerator
{
private:
    string map_adress = "..//..//Database//map.txt";

public:
    //TODO
    void generateMap(Map &map) override;
    void setMapAdress(string adress);
};


#endif //FANCOURIER_FILEMAPLOADER_H