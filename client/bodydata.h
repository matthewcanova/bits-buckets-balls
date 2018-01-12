#ifndef BODYDATA_H
#define BODYDATA_H

#include <string>

struct bodydata
{
    void* sprite;   // This will be cast to a QGraphicsPixmapItem pointer
    std::string name;
    void* world;    // This will be cast to a WorldSim pointer
    int ballCount = 0;
};

#endif // BODYDATA_H
