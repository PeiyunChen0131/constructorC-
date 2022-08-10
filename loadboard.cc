#include <memory>
#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include "loadboard.h"
#include "edge.h"
#include "tile.h"
#include "vertex.h"
#include "player.h"
using std::ifstream;
using std::istream;

void LoadBoard::createBoard(unsigned seed, std::string fname) {
    std::string input;
    int resourceNum;
    int tileValue;

    std::ifstream in;
    in.exceptions(std::ifstream::badbit);
    //std::shared_ptr<istream> in = std::make_shared<ifstream>( fname );

    std::string s;
    in = std::ifstream{fname};
    std::getline(in, s);
    std::istringstream ss1{s};
    ss1 >> input;
    whosTurn = std::stoi(input);

    std::string playerOne;
    std::getline(in, playerOne);
    std::string playerTwo;
    std::getline(in, playerTwo);
    std::string playerThree;
    std::getline(in, playerThree);
    std::string playerFour;
    std::getline(in, playerFour);

    std::getline(in, s);
    ss1 = std::istringstream{s};
    for (int i = 0; i < 19; ++i) {
        ss1 >> input;
        resourceNum = std::stoi(input);
        ss1 >> input;
        tileValue = std::stoi(input);

        std::shared_ptr<Tile> tile = std::make_shared<Tile>(i, resourceNum, tileValue);
        //tile.addAdjEdges();

        theTiles.emplace_back(tile);
    }

    std::getline(in, s);
    ss1 = std::istringstream{s};
    ss1 >> input;
    geeseIsAt = std::stoi(input);

    initBoard();

    int resNum;
    std::string resType;

    for (int i = 0; i < 4; ++i) {
        std::shared_ptr<Player> player;
	if (i == 0) {
            player = std::make_shared<Player>("Blue");
	    ss1 = std::istringstream{playerOne};
        } else if (i == 1) {
            player = std::make_shared<Player>("Red");
	    ss1 = std::istringstream{playerTwo};
        } else if (i == 2) {
            player = std::make_shared<Player>("Orange");
	    ss1 = std::istringstream{playerThree};
        } else if (i == 3) {
            player = std::make_shared<Player>("Yellow");
	    ss1 = std::istringstream{playerFour};
        }

        for (int x = 0; x < 5; ++x) {
            ss1 >> input;
            resourceNum = std::stoi(input);
            player->loadResource(x, resourceNum);
        }

	thePlayer.emplace_back(player);
	ss1 >> input;
            while (ss1 >> input) {
                if (input == "h") {
                    break;
                }
                resNum = std::stoi(input);
                boardBuildRes(i, resNum, "Road","FORCEBUILD");
            }
            while (ss1 >> input) {
                resNum = std::stoi(input);
                ss1 >> resType;
                if (resType == "B") {
                    boardBuildRes(i,resNum,"Basement","FORCEBUILD");
                } else if (resType == "T") {
                    boardBuildRes(i,resNum,"Tower","FORCEBUILD");
                } else if (resType == "H") {
                    boardBuildRes(i,resNum,"House","FORCEBUILD");
                }
            }
    }
}
