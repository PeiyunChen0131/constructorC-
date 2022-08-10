#ifndef _TOWERSTRATEGY_H_
#define _TOWERSTRATEGY_H_
#include "buildingstrategy.h"

class TowerStrategy : public BuildingStrategy{
    std::string name = "T";

    public:
    Resource strategyGiveRes(Resource res) override;

    std::string getName() override;
};

#endif
