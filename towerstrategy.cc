#include "towerstrategy.h"

Resource TowerStrategy::strategyGiveRes(Resource res) {
    return res + res + res;
}

std::string TowerStrategy::getName() {
    return name;
}
