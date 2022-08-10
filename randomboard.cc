#include <memory>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include "randomboard.h"
#include "edge.h"
#include "tile.h"
#include "vertex.h"
#include "player.h"

void RandomBoard::createBoard(unsigned seed, std::string fname) {
    std::string input;
    int resourceNum;
    int tileValue;

    std::vector<int> resource = { 4, 4, 4, 3, 3, 3, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 5 };
    std::vector<int> value = { 2, 3, 3, 4, 4, 5, 5, 6, 6, 8, 8, 9, 9, 10, 10, 11, 11, 12 };

    std::default_random_engine rng{seed};

    for ( int i = 0; i < 1000; i++ ) {
        std::shuffle( resource.begin(), resource.end(), rng );
        std::shuffle( value.begin(), value.end(), rng );
    }

    int count = 0;
    for (int i = 0; i < 19; ++i) {
        resourceNum = resource[i];
        if (resourceNum == 5) {
            tileValue = 7;
        } else {
            tileValue = value[count];
            count++;
        }

        std::shared_ptr<Tile> tile = std::make_shared<Tile>(i, resourceNum, tileValue);
        theTiles.emplace_back(tile);
    }

    std::shared_ptr<Player> player1 = std::make_shared<Player>("Blue");
    thePlayer.emplace_back(player1);
    std::shared_ptr<Player> player2 = std::make_shared<Player>("Red");
    thePlayer.emplace_back(player2);
    std::shared_ptr<Player> player3 = std::make_shared<Player>("Orange");
    thePlayer.emplace_back(player3);
    std::shared_ptr<Player> player4 = std::make_shared<Player>("Yellow");
    thePlayer.emplace_back(player4);

    initBoard();
}
