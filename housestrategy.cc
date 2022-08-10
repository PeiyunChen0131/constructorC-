#include "housestrategy.h"

Resource HouseStrategy::strategyGiveRes(Resource res) {
    return res + res;
}

std::string HouseStrategy::getName() {
    return name;
}
