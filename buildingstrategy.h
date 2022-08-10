#ifndef _BUILDINGSTRATEGY_H_
#define _BUILDINGSTRATEGY_H_
#include "resource.h"

class BuildingStrategy {
    public:
    virtual Resource strategyGiveRes(Resource res) = 0;
    virtual std::string getName() = 0;
};

#endif
