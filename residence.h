#ifndef _RESIDENCE_H_
#define _RESIDENCE_H_
#include <memory>
#include "resource.h"
#include "buildingstrategy.h"

class Residence {
    std::shared_ptr<BuildingStrategy> _strategy;

    public:
    Residence(std::shared_ptr<BuildingStrategy> strategy);

    void setStrategy(std::shared_ptr<BuildingStrategy> strategy);

    Resource residenceGiveRes(Resource res);

    std::string getName();
};

#endif
