#ifndef FANCOURIER_FILEMAPLOADER_H
#define FANCOURIER_FILEMAPLOADER_H
#include "IMapGenerator.h"

class FileMapLoader : public IMapGenerator
{
private:
    string config_file_adress;
    ifstream config_file;
public:
    //TODO
    void setConfigFile(string adress) override;
    void generateMap(Map &map) override;
};


#endif //FANCOURIER_FILEMAPLOADER_H