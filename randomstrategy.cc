#include <random>
#include "randomstrategy.h"

int RandomStrategy::rollDice() {
    int dice = rand() % 11 + 2;
    return dice;
}
