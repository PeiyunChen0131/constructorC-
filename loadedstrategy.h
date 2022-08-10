#ifndef _LOADEDSTRATEGY_H_
#define _LOADEDSTRATEGY_H_
#include "dicestrategy.h"

class LoadedStrategy : public DiceStrategy {
    public:
    virtual int rollDice() override;
};

#endif
