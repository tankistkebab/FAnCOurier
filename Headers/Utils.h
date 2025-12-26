#ifndef FANCOURIER_UTILS_H
#define FANCOURIER_UTILS_H

enum class AGENTSTATUS
{
    //TODO
};

enum class CELLTYPE
{
    ROAD, //.
    WALL, //#
    BASE, //B
    CHARGING_STATION, //S
    CLIENT, //D
    EMPTY_SPACE // X
};

typedef struct POINT
{
    int x, y;
    bool operator==(const POINT& pct) const // copiem doar adresa POINT pt eficienta
    {
        return ((x == pct.x) && (y == pct.y));
    }
};

#endif //FANCOURIER_UTILS_H