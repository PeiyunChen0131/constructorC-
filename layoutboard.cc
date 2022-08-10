#include <memory>
#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include "layoutboard.h"
#include "edge.h"
#include "tile.h"
#include "vertex.h"
#include "player.h"
using std::ifstream;
using std::istream;

void LayoutBoard::createBoard(unsigned seed, std::string fname) {
	std::string input;
        int resourceNum;
        int tileValue;

        //std::file.exceptions(std::ifstream::badbit);
        std::shared_ptr<istream> in = std::make_shared<ifstream>( fname );

        for (int i = 0; i < 19; ++i) {
            *in >> input;
            resourceNum = std::stoi(input, nullptr);
            *in >> input;
            tileValue = std::stoi(input, nullptr);
            
            std::shared_ptr<Tile> tile = std::make_shared<Tile>(i, resourceNum, tileValue);
            //tile.addAdjEdges();
            
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
