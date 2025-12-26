#ifndef FANCOURIER_FILEMAPLOADER_H
#define FANCOURIER_FILEMAPLOADER_H
#include "IMapGenerator.h"
#include <fstream>

class FileMapLoader : public IMapGenerator
{
private:
    ifstream file = nullptr;
public:

    void setFileAdress(string adresa);

    void generateMap(Map &map) override;
};


#endif //FANCOURIER_FILEMAPLOADER_H