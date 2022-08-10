#include <memory>
#include "dice.h"
#include <iostream>

Dice::Dice(std::shared_ptr<DiceStrategy> strategy) : _strategy{strategy} { 
    //std::cout << "creating dice" << std::endl;
}
// dbc memoryleak virtual destructor

void Dice::setStrategy(std::shared_ptr<DiceStrategy> strategy) {
    //std::cout << "setting dice" << std::endl;
    this->_strategy = strategy;
}

int Dice::rollDice() {
        //std::cout << "rollDice" << std::endl;
    return this->_strategy->rollDice();
}
