#ifndef FANCOURIER_PROCEDURALMAPGENERATOR_H
#define FANCOURIER_PROCEDURALMAPGENERATOR_H
#include "IMapGenerator.h"


class ProceduralMapGenerator : public IMapGenerator
{
private:
    string config_file_adress;
public:
    void generateMap(Map &map) override;
    void setConfigFile(string adress) override;
};


#endif //FANCOURIER_PROCEDURALMAPGENERATOR_H