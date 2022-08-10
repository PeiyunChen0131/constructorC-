#ifndef _TILE_H_
#define _TILE_H_
#include <memory>
#include <set>
#include "subject.h"
#include "player.h"
#include "resource.h"

class Tile : public Subject {
    int tileNumber;
    const int resNum;
    int tileValue;
    bool geese;
    Resource resource;

    public:
    Tile(int number, int resource, int value);

    int getResNum();

    Resource getResource();

    int getTileValue();

    //std::shared_ptr<Player> findPlayer(std::string colour);

    std::set<std::string> tileWhoToSteal();
};

#endif
