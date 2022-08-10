#ifndef _DICE_H_
#define _DICE_H_
#include <memory>
#include "dicestrategy.h"

class Dice {
    std::shared_ptr<DiceStrategy> _strategy;

    public:
    Dice(std::shared_ptr<DiceStrategy> strategy);

    void setStrategy(std::shared_ptr<DiceStrategy> strategy);

    int rollDice();
};

#endif
