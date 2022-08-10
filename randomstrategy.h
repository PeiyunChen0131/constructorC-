#ifndef _RANDOMSTRATEGY_H_
#define _RANDOMSTRATEGY_H_
#include "dicestrategy.h"

class RandomStrategy : public DiceStrategy {
    public:
    virtual int rollDice() override;
};

#endif
