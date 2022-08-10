#ifndef _HOUSESTRATEGY_H_
#define _HOUSESTRATEGY_H_
#include "buildingstrategy.h"

class HouseStrategy : public BuildingStrategy{
    std::string name = "H";

    public:
    Resource strategyGiveRes(Resource res) override;

    std::string getName() override;
};

#endif
