#ifndef _BASEMENTSTRATEGY_H_
#define _BASEMENTSTRATEGY_H_
#include "buildingstrategy.h"

class BasementStrategy : public BuildingStrategy{
    std::string name = "B";
  
    public:
    Resource strategyGiveRes(Resource res) override;

    std::string getName() override;
};

#endif
