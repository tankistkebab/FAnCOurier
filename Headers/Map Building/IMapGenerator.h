#ifndef FANCOURIER_IMAPGENERATOR_H
#define FANCOURIER_IMAPGENERATOR_H

//Interfata, denumita context in paradigma strategy
class IMapGenerator
{
    public virtual void generateMap(Map& map) = 0;
    virtual ~IMapGenerator();
};


#endif //FANCOURIER_IMAPGENERATOR_H