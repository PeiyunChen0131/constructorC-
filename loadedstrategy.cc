#include <iostream>
#include "loadedstrategy.h"

int LoadedStrategy::rollDice() {
    int dice = 0;
    //std::cout << "realrolldice" << std::endl;
    while(true) {
        // dbc exception safe: if rolled 7 then save
        try {
            std::cout << "Input a roll between 2 and 12:" << std::endl;
            std::cout << ">";
            std::cin >> dice;
            if (dice >= 2 && dice <= 12) {
                return dice;
            }
            std::cout << "Invalid roll." << dice << std::endl;
        // dbc try-catch here
        } catch (std::ios::failure &error) {
            if (std::cin.eof()) {
                throw error;
            } else {
                std::cin.clear();
                std::cin.ignore();
                std::cout << "Invalid roll." << std::endl;
            }
        }
    }
}
