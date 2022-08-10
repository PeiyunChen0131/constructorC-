#include <memory>
#include <string>
#include <set>
#include "resource.h"
#include "tile.h"

Tile::Tile(int number, int resource, int value)
    : tileNumber{number}, resNum{resource}, tileValue{value}, geese{false} 
{
	this->resource.addThisResource(resource, 1);    
}

int Tile::getResNum() {
    return resNum;
}

Resource Tile::getResource() {
	return resource;
}

int Tile::getTileValue() {
    return tileValue;
}

/*
std::shared_ptr<Player> Tile::findPlayer(std::string colour) {
    std::shared_ptr<Player> n = std::make_shared<Player>();
    return n;
}
*/

std::set<std::string> Tile::tileWhoToSteal() {
        std::set<std::string> toRet;
        for (auto n : observers) {  // dbc vertices here
            if (n->getOwner()) {
		if (n->getOwner()->playerHasResource()) {
                    toRet.insert(n->getOwner()->getName());
		}
            }
        }
        return toRet;
}
