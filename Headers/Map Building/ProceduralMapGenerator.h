#ifndef FANCOURIER_PROCEDURALMAPGENERATOR_H
#define FANCOURIER_PROCEDURALMAPGENERATOR_H
#include "IMapGenerator.h"


class ProceduralMapGenerator : public IMapGenerator
{
public:
    void generateMap(Map &map) override;
};


#endif //FANCOURIER_PROCEDURALMAPGENERATOR_H