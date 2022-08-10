#include <iostream>
#include <string>
#include <fstream>
#include <memory>
#include <vector>
#include "edge.h"
#include "tile.h"
#include "vertex.h"
#include "board.h"
#include "resource.h"
using std::cout;
using std::endl;
using std::to_string;

int Board::playerNum() {
	return howManyPlayer;
}

bool Board::placeGeese(int geeseAtTile) {
    // check if place geese here is allowed
    // erase previous geese
    if (geeseIsAt != geeseAtTile && geeseAtTile >= 0 && geeseAtTile <= 18) {
        geeseIsAt = geeseAtTile;
        return true;
    } else {
        return false;
    }
}

std::string Board::getPlayerNameAt(int n) {
    if (n >= 0 && n <=3) {
        if (n==0) {
            return "Blue";
        } else if (n == 1) {
            return "Red";

        } else if (n==2) {
            return "Orange";
        } else if (n ==3) {
            return "Yellow";


        }

    }
    return "nothing";
}

int Board::getWhosTurn() {
	return whosTurn;
}

std::string Board::getPlayerStatus(int num) {
    return thePlayer.at(num)->playerStatus();
}

void Board::printPlayerRes(int num) {
    int numOfRes = thePlayer.at(num)->getResNum();
    for (int i = 0; i < numOfRes; ++i) {
        std::cout << thePlayer.at(num)->getRes(i) << std::endl;
    }
}

bool Board::checkPts(int num) {
    if (thePlayer.at(num)->getPoint() >= 10) {
        return true;
    }
    return false;
}

void Board::boardLoseResources() {
    for (auto n : thePlayer) {
        n->playerLoseResources();
    }
}

std::set<std::string> Board::boardWhoToSteal() {

    return theTiles.at(geeseIsAt)->tileWhoToSteal();
}

void Board::boardSteal(std::string colour, int giveTo) {
    int playerNum = colourToInt(colour);
    if (playerNum < 0) {int n = 0; throw n;} //dbc this
    std::string res = thePlayer.at(playerNum)->playerSteal();
    std::cout << "Builder " << thePlayer.at(giveTo)->getName() << " steals ";
    std::cout << res;
    std::cout << " from " << thePlayer.at(playerNum)->getName() << "." << std::endl;
    thePlayer.at(giveTo)->playerStealed(res);
}

void Board::distributeResources(int tileValue) {
    for (auto n : theTiles) {
        if (n->getTileValue() == tileValue) {
            n->notifyObservers();

        }

        // if (n->getTileValue() == tileValue)
        // n->actualDistributeResources (print in actualDistributeResources)
    }
}

std::string Board::boardBuildRes(int whosTurn, int resNum, std::string resType, std::string stage) {
    if (resType == "Road") {
        int edgeSize = theEdges.size();
        if (resNum < 0 || resNum > edgeSize) {
            return "CanNotBuild";
        }
    }
    int verticesSize = theVertices.size();
    // player also keep track of the vertices
    if (resNum >= 0 && resNum < verticesSize) {
        if (stage == "FORCEBUILD") { //always returns success
            if (resType == "Road") {
                theEdges.at(resNum)->forceBuildRoad(thePlayer.at(whosTurn));
                thePlayer.at(whosTurn)->playerAddEdge(theEdges.at(resNum));
                return "Success";
            } else {
                theVertices.at(resNum)->forceBuild(thePlayer.at(whosTurn), resType);
                thePlayer.at(whosTurn)->playerAddVertex(theVertices.at(resNum));
                return "Success";
            }
        } else if (stage == "STARTOFGAME") { // no "Road" in here
            bool success = theVertices.at(resNum)->startBuild(thePlayer.at(whosTurn));
            if (success) {
                thePlayer.at(whosTurn)->playerAddVertex(theVertices.at(resNum));
                return "Success";
            } else {
                return "CanNotBuild";
            }
        } else if (stage == "DURINGTURN") {
            bool success = thePlayer.at(whosTurn)->playerCheckResource(resType, "Check");
            if (success) {
                if (resType == "Road") {
                    bool buildSuccess = theEdges.at(resNum)->buildRoad(thePlayer.at(whosTurn));
                    if (buildSuccess) {
                        thePlayer.at(whosTurn)->playerAddEdge(theEdges.at(resNum));
                        return "Success";
                    } else {
                        return "CanNotBuild";
                    }
                } else {
                    bool buildSuccess = theVertices.at(resNum)->buildRes(thePlayer.at(whosTurn), resType);
                    //std::cout << "here" << std::endl;
                    if (buildSuccess) {
                        //std::cout << "here" << std::endl;
                        thePlayer.at(whosTurn)->playerCheckResource(resType, "Build");
                        if (resType == "Basement") {
                            thePlayer.at(whosTurn)->playerAddVertex(theVertices.at(resNum));
                        }
                        return "Success";
                    } else {
                        return "CanNotBuild";
                    }
                }
            } else {
                return "NotEnoughResource";
            }
        } else {
            return "CanNoteBuild";
        }
    } else {
        return "CanNotBuild";
    }
}

std::string Board::boardImprove(int turn, int resNum) {
    int vertexSize = theVertices.size();
    if (resNum >= 0 && resNum < vertexSize) {
        if (theVertices.at(resNum)->getResidenceName() == "empty") {
            return "CanNotBuild";
        } else if (theVertices.at(resNum)->getResidenceName() == "B") {
            return boardBuildRes(turn,resNum,"House","DURINGTURN");
        } else if (theVertices.at(resNum)->getResidenceName() == "H") {
            return boardBuildRes(turn,resNum,"Tower","DURINGTURN");
        }
    }
    return "CanNotBuild";
}

int Board::colourToInt(std::string colour) {
    if (colour == "Blue") {
        return 0;
    } else if (colour == "Red") {
        return 1;
    } else if (colour == "Orange") {
        return 2;
    } else if (colour == "Yellow") { // dbc else if here (colour == "Yellow")
        return 3;
    } else {
        return -1;
    }
}
//
std::string Board::intToColour(int input) {
    if (input == 0) {
        return "Blue";
    } else if (input == 1) {
        return "Red";
    } else if (input == 2) {
        return "Orange";
    } else if (input == 3) {
        return "Yellow";
    } else {
        return "NoColour";
    }
}

//Trade
bool Board::boardCheckTrade(int playerNum, std::string resource) {
    return thePlayer.at(playerNum)->playerCheckTrade(resource);
}

void Board::boardTrade(int whosTurn,int toPlayer,std::string give,std::string take) {
    thePlayer.at(whosTurn)->playerTrade(give, take);
    thePlayer.at(toPlayer)->playerTrade(take, give);
    std::cout << "Traded" << std::endl;
}

void Board::boardSave(std::string outFile, int whosTurn) {
    std::ofstream out{outFile};
    out << whosTurn << std::endl;
    for (auto n : thePlayer) {
        std::string resAndResource = n->playerSave();
        out << resAndResource << std::endl;
    }
    std::string allTile = "";
    for (auto n : theTiles) {
        allTile = allTile + n->getResource().resourceOnTile(); //dbc here
        allTile = allTile + " " + std::to_string(n->getTileValue()) + " ";
    }
    out << allTile << std::endl;
    out << geeseIsAt << std::endl;
}

void Board::boardSetDice(int turn, std::string diceType) {
    //std::cout <<"boardsetdice" << std::endl;
    thePlayer.at(turn)->playerSetDice(diceType);
}

int Board::boardRollDice(int turn) {
    return thePlayer.at(turn)->playerRollDice();
}

void printIntSp(int num) {
    if (num < 10) {
        cout << " ";
    }
    cout << num;
}

void printResource(int num) {
	Resource r;
	cout << "  ";
	if (num == 1) {
		cout << r.toStr(num);
	} else if (num == 0 || num == 2) {
		cout << r.toStr(num) << " ";
	} else {
		cout << r.toStr(num) << "  ";
	}
}

void Board::printTopEdge(int *vertice, int *edge) {
    const std::string doubMins = "--";
    const std::string verBar = "|";

    cout << verBar;
    cout << theVertices[*vertice]->getDisplay();
    (*vertice)++;
    cout << verBar << doubMins;
    cout << theEdges[*edge]->getDisplay();
    cout << doubMins << verBar;
    cout << theVertices[*vertice]->getDisplay();
    cout << verBar;
}

void Board::printBoard() {
    const std::string fl = "                    ";
    const std::string sl = "          ";
    const std::string fEmp = "                      |         |                     ";
    //const std::string sEmp = "            |         |         |         |           ";
    //const std::string tEmp = "  |         |         |         |         |         | ";
    const std::string tileCap = "        ";
    const std::string tileCapW = " |         |";
    const std::string verBar = "|";

    int curTile = 0;
    int curVertice = 0;
    int curEdge = 0;

    //std::vector<std::vector<std::string>> row;
    for (int i = 1; i < 42; ++i) {
        //std::vector<std::string> c;
        cout << "      ";
        if (i == 1 || i == 41) {
            //c.emplace_back(fl);
            cout << fl;
            printTopEdge(&curVertice, &curEdge);
            cout << fl;
            curVertice++;
            curEdge++;
        } else if (i == 2 || i == 40) {
            cout << fEmp;
        } else if (i == 3) {
            cout << fl << " ";
            cout << theEdges[curEdge]->getDisplay();
            curEdge++;
            cout << "   ";
            printIntSp(curTile);
            cout << "   ";
            cout << theEdges[curEdge]->getDisplay();
            curEdge++;
            cout << " " << fl;
        } else if (i == 4) {
            cout << fl << "  |";
            printResource(theTiles[curTile]->getResNum());
            cout << " | " << fl;
        } else if (i == 5 || i == 37) {
            cout << sl;
            printTopEdge(&curVertice, &curEdge);
            curVertice++;
            curEdge++;
            cout << "  ";
            if (theTiles[curTile]->getTileValue() == 7) {
                cout << "  ";
            } else {
                printIntSp(theTiles[curTile]->getTileValue());
            }
            cout << "  ";
            printTopEdge(&curVertice, &curEdge);
            curVertice++;
            curEdge++;
            cout << sl;
        } else if (i == 6) {
            if (curTile == geeseIsAt) {
                cout << "            |         |  GEESE  |         |           ";
            } else {
                cout << "            |         |         |         |           ";
            }
        } else if (i == 7) {
            cout << sl << " ";
            cout << theEdges[curEdge]->getDisplay();
            curEdge++;
            cout << "   ";
            printIntSp(curTile);
            cout << "   ";
            cout << theEdges[curEdge]->getDisplay();
            curEdge++;
            cout << tileCap;
            cout << theEdges[curEdge]->getDisplay();
            curEdge++;
            cout << "   ";
            printIntSp(curTile + 1);
            cout << "   ";
            cout << theEdges[curEdge]->getDisplay();
            curEdge++;
            cout << " " << sl;
        } else if (i == 8) {
            cout << "            |";
            printResource(theTiles[curTile]->getResNum());
            cout << tileCapW;
            printResource(theTiles[curTile+1]->getResNum());
            cout << " |           ";
        } else if (i == 9 || i == 17 || i == 25 || i == 33) {
            for (int x = 0; x < 2; ++x) {
                printTopEdge(&curVertice, &curEdge);
                curVertice++;
                curEdge++;
                cout << "  ";
                if (theTiles[curTile+x]->getTileValue() == 7) {
                    cout << "  ";
                } else {
                    printIntSp(theTiles[curTile+x]->getTileValue());
                }
                cout << "  ";
            }
            printTopEdge(&curVertice, &curEdge);
            curVertice++;
            curEdge++;
            cout << "  ";
        } else if (i == 10 || i == 18 || i == 26) {
            if (curTile == geeseIsAt) {
                cout << "  |         |  GEESE  |         |         |         | ";
            } else if ((curTile + 1) == geeseIsAt) {
                cout << "  |         |         |         |  GEESE  |         | ";
            } else {
                cout << "  |         |         |         |         |         | ";
            }
        } else if (i == 11 || i == 19 || i == 27) {
            cout << " ";
            for (int x = 0; x < 3; ++x) {
                cout << theEdges[curEdge]->getDisplay();
                curEdge++;
                cout << "   ";
                printIntSp(curTile+x);
                cout << "   ";
                cout << theEdges[curEdge]->getDisplay();
                curEdge++;
                if (x < 2) {
                    cout << tileCap;
                }
            }
            cout << " ";
        }  else if (i == 12 || i == 20 || i == 28) {
            cout << "  |";
            for (int x = 0; x < 3; ++x) {
                printResource(theTiles[curTile+x]->getResNum());
                if (x < 2) {
                    cout << tileCapW;
                }
            }
            cout << " | ";
        } else if (i == 13 || i == 21 || i ==29) {
            cout << "|" << theVertices[curVertice]->getDisplay() << "|";
            curVertice++;
            for (int x = 0; x < 3; ++x) {
                cout << "  ";
                if (theTiles[curTile+x]->getTileValue() == 7) {
                    cout << "  ";
                } else {
                    printIntSp(theTiles[curTile+x]->getTileValue());
                }
                cout << "  ";
                if (x < 2) {
                    printTopEdge(&curVertice, &curEdge);
                    curVertice++;
                    curEdge++;
                }
            }
            cout << "|" << theVertices[curVertice]->getDisplay() << "|";
            curVertice++;
        } else if (i == 14 || i == 22 || i == 30) {
            if (curTile == geeseIsAt) {
                cout << "  |  GEESE  |         |         |         |         | ";
            } else if ((curTile + 1) == geeseIsAt) {
                cout << "  |         |         |  GEESE  |         |         | ";
            } else if ((curTile + 2) == geeseIsAt) {
                cout << "  |         |         |         |         |  GEESE  | ";
            } else {
                cout << "  |         |         |         |         |         | ";
            }
        } else if (i == 15 || i == 23 || i == 31) {
            cout << " ";
            for (int x = 0; x < 3; ++x) {
                cout << theEdges[curEdge]->getDisplay();
                curEdge++;
                cout << tileCap;
                cout << theEdges[curEdge]->getDisplay();
                curEdge++;
                if (x < 2) {
                    cout << "   ";
                    printIntSp(curTile+x);
                    cout << "   ";
                }
            }
            cout << " ";
        } else if (i == 16 || i == 24 || i == 32) {
            cout << " ";
	    for (int x = 0; x < 3; ++x) {
                cout << tileCapW;
                if (x < 2) {
                    printResource(theTiles[curTile+x]->getResNum());
                }
            }
            cout << " ";
        } else if (i == 34) {
            if (curTile == geeseIsAt) {
                cout << "            |  GEESE  |         |         |           ";
            } else if ((curTile + 1) == geeseIsAt) {
                cout << "            |         |         |  GEESE  |           ";
            } else {
                cout << "            |         |         |         |           ";
            }
        } else if (i == 35) {
            cout << "           ";
            for (int x = 0; x < 2; ++x) {
                cout << theEdges[curEdge]->getDisplay();
                curEdge++;
                cout << tileCap;
                cout << theEdges[curEdge]->getDisplay();
                curEdge++;
                if (x < 1) {
                    cout << "   ";
                    printIntSp(curTile+x);
                    cout << "   ";
                }
            }
            cout << "           ";
        } else if (i == 36) {
            cout << "            |         |";
            printResource(theTiles[curTile]->getResNum());
            cout << " |         |           ";
        } else if (i == 38) {
            if (curTile == geeseIsAt) {
                cout << "                      |  GEESE  |                     ";
            } else {
                cout << "                      |         |                     ";
            }
        } else if (i == 39) {
            cout << "                     ";
            cout << theEdges[curEdge]->getDisplay();
            curEdge++;
            cout << "        ";
            cout << theEdges[curEdge]->getDisplay();
            curEdge++;
            cout << "                     ";
        }

        cout << endl;
        if (i % 4 == 2 && i != 2) {
            if (i == 6) {
                curTile++;
            } else if (i == 10 || i == 18 || i == 26 || i == 34) {
                curTile += 2;
            } else {
                curTile += 3;
            }
        }
        //row.emplace_back(c);
    }

    /*
    // check connectivity for debugging (remove after finish)
    int tl = theTiles.size();
    int vl = theVertices.size();
    int el = theEdges.size();

    cout << endl;
    for (int i = 0; i < tl; ++i) {
        cout << theTiles[i]->getTileValue() << ": ";
        cout << theTiles[i]->getObserverNames();
        cout << endl;
    }

    cout << endl;
    for (int i = 0; i < vl; ++i) {
        cout << theVertices[i]->getDisplay() << ": ";
        theVertices[i]->printInfo();
        cout << endl;
    }

    cout << endl;
    for (int i = 0; i < el; ++i) {
        cout << theEdges[i]->getDisplay() << ": ";
        theEdges[i]->printInfo();
        cout << endl;
    }
    */
}

void Board::initBoard() {
    std::string str;
    for (int i = 0; i < 54; ++i) {
        str = to_string(i);
        if (i < 10) {
            str.insert(0, " ");
        }
        theVertices.emplace_back(std::make_shared<Vertex>(i, str));
    }

    for (int i = 0; i < 72; ++i) {
        str = to_string(i);
        if (i < 10) {
            str.insert(0, " ");
        }
        theEdges.emplace_back(std::make_shared<Edge>(i, str));
    }

    // connecting vertices with vertices
    theVertices[0]->addAdjVertices(theVertices[1].get());
    theVertices[0]->addAdjVertices(theVertices[3].get());
    theVertices[1]->addAdjVertices(theVertices[0].get());
    theVertices[1]->addAdjVertices(theVertices[4].get());
    theVertices[2]->addAdjVertices(theVertices[3].get());
    theVertices[2]->addAdjVertices(theVertices[7].get());
    theVertices[3]->addAdjVertices(theVertices[0].get());
    theVertices[3]->addAdjVertices(theVertices[2].get());
    theVertices[3]->addAdjVertices(theVertices[8].get());
    theVertices[4]->addAdjVertices(theVertices[1].get());
    theVertices[4]->addAdjVertices(theVertices[5].get());
    theVertices[4]->addAdjVertices(theVertices[9].get());
    theVertices[5]->addAdjVertices(theVertices[4].get());
    theVertices[5]->addAdjVertices(theVertices[10].get());
    theVertices[6]->addAdjVertices(theVertices[7].get());
    theVertices[6]->addAdjVertices(theVertices[12].get());
    theVertices[7]->addAdjVertices(theVertices[2].get());
    theVertices[7]->addAdjVertices(theVertices[6].get());
    theVertices[7]->addAdjVertices(theVertices[13].get());
    theVertices[8]->addAdjVertices(theVertices[3].get());
    theVertices[8]->addAdjVertices(theVertices[9].get());
    theVertices[8]->addAdjVertices(theVertices[14].get());
    theVertices[9]->addAdjVertices(theVertices[4].get());
    theVertices[9]->addAdjVertices(theVertices[8].get());
    theVertices[9]->addAdjVertices(theVertices[15].get());
    theVertices[10]->addAdjVertices(theVertices[5].get());
    theVertices[10]->addAdjVertices(theVertices[11].get());
    theVertices[10]->addAdjVertices(theVertices[16].get());
    theVertices[11]->addAdjVertices(theVertices[10].get());
    theVertices[11]->addAdjVertices(theVertices[17].get());
    for (int i = 12; i < 42; ++i) {
        theVertices[i]->addAdjVertices(theVertices[i-6].get());
        theVertices[i]->addAdjVertices(theVertices[i+6].get());
    }
    theVertices[13]->addAdjVertices(theVertices[14].get());
    theVertices[14]->addAdjVertices(theVertices[13].get());
    theVertices[15]->addAdjVertices(theVertices[16].get());
    theVertices[16]->addAdjVertices(theVertices[15].get());
    theVertices[18]->addAdjVertices(theVertices[19].get());
    theVertices[19]->addAdjVertices(theVertices[18].get());
    theVertices[20]->addAdjVertices(theVertices[21].get());
    theVertices[21]->addAdjVertices(theVertices[20].get());
    theVertices[22]->addAdjVertices(theVertices[23].get());
    theVertices[23]->addAdjVertices(theVertices[22].get());
    theVertices[25]->addAdjVertices(theVertices[26].get());
    theVertices[26]->addAdjVertices(theVertices[25].get());
    theVertices[27]->addAdjVertices(theVertices[28].get());
    theVertices[28]->addAdjVertices(theVertices[27].get());
    theVertices[30]->addAdjVertices(theVertices[31].get());
    theVertices[31]->addAdjVertices(theVertices[30].get());
    theVertices[32]->addAdjVertices(theVertices[33].get());
    theVertices[33]->addAdjVertices(theVertices[32].get());
    theVertices[34]->addAdjVertices(theVertices[35].get());
    theVertices[35]->addAdjVertices(theVertices[34].get());
    theVertices[37]->addAdjVertices(theVertices[38].get());
    theVertices[38]->addAdjVertices(theVertices[37].get());
    theVertices[39]->addAdjVertices(theVertices[40].get());
    theVertices[40]->addAdjVertices(theVertices[39].get());
    for (int i = 42; i < 48; ++i) {
        theVertices[i]->addAdjVertices(theVertices[i-6].get());
        if (i % 2 == 0) {
            theVertices[i]->addAdjVertices(theVertices[i+1].get());
            theVertices[i+1]->addAdjVertices(theVertices[i].get());
        }
    }
    for (int i = 48; i < 52; ++i) {
        theVertices[i]->addAdjVertices(theVertices[i-5].get());
        theVertices[i-5]->addAdjVertices(theVertices[i].get());
        if (i % 2 == 0) {
            theVertices[i]->addAdjVertices(theVertices[i+1].get());
            theVertices[i+1]->addAdjVertices(theVertices[i].get());
        }
    }
    theVertices[49]->addAdjVertices(theVertices[52].get());
    theVertices[52]->addAdjVertices(theVertices[49].get());
    theVertices[50]->addAdjVertices(theVertices[53].get());
    theVertices[53]->addAdjVertices(theVertices[50].get());
    theVertices[52]->addAdjVertices(theVertices[53].get());
    theVertices[53]->addAdjVertices(theVertices[52].get());

    // connecting vertices with edges
    theEdges[0]->addAdjVertices(theVertices[0].get());
    theEdges[0]->addAdjVertices(theVertices[1].get());
    theEdges[1]->addAdjVertices(theVertices[0].get());
    theEdges[1]->addAdjVertices(theVertices[3].get());
    theEdges[2]->addAdjVertices(theVertices[1].get());
    theEdges[2]->addAdjVertices(theVertices[4].get());
    theEdges[3]->addAdjVertices(theVertices[2].get());
    theEdges[3]->addAdjVertices(theVertices[3].get());
    theEdges[4]->addAdjVertices(theVertices[4].get());
    theEdges[4]->addAdjVertices(theVertices[5].get());
    for (int i = 5; i < 9; ++i) {
        theEdges[i]->addAdjVertices(theVertices[i-3].get());
        theEdges[i]->addAdjVertices(theVertices[i+2].get());
    }
    theEdges[9]->addAdjVertices(theVertices[6].get());
    theEdges[9]->addAdjVertices(theVertices[7].get());
    theEdges[10]->addAdjVertices(theVertices[8].get());
    theEdges[10]->addAdjVertices(theVertices[9].get());
    theEdges[11]->addAdjVertices(theVertices[10].get());
    theEdges[11]->addAdjVertices(theVertices[11].get());
    for (int i = 12; i < 18; ++i) {
        theEdges[i]->addAdjVertices(theVertices[i-6].get());
        theEdges[i]->addAdjVertices(theVertices[i].get());
    }
    theEdges[18]->addAdjVertices(theVertices[13].get());
    theEdges[18]->addAdjVertices(theVertices[14].get());
    theEdges[19]->addAdjVertices(theVertices[15].get());
    theEdges[19]->addAdjVertices(theVertices[16].get());
    for (int i = 20; i < 26; ++i) {
        theEdges[i]->addAdjVertices(theVertices[i-8].get());
        theEdges[i]->addAdjVertices(theVertices[i-2].get());
    }
    theEdges[26]->addAdjVertices(theVertices[18].get());
    theEdges[26]->addAdjVertices(theVertices[19].get());
    theEdges[27]->addAdjVertices(theVertices[20].get());
    theEdges[27]->addAdjVertices(theVertices[21].get());
    theEdges[28]->addAdjVertices(theVertices[22].get());
    theEdges[28]->addAdjVertices(theVertices[23].get());
    for (int i = 29; i < 35; ++i) {
        theEdges[i]->addAdjVertices(theVertices[i-11].get());
        theEdges[i]->addAdjVertices(theVertices[i-5].get());
    }
    theEdges[35]->addAdjVertices(theVertices[25].get());
    theEdges[35]->addAdjVertices(theVertices[26].get());
    theEdges[36]->addAdjVertices(theVertices[27].get());
    theEdges[36]->addAdjVertices(theVertices[28].get());
    for (int i = 37; i < 43; ++i) {
        theEdges[i]->addAdjVertices(theVertices[i-13].get());
        theEdges[i]->addAdjVertices(theVertices[i-7].get());
    }
    theEdges[43]->addAdjVertices(theVertices[30].get());
    theEdges[43]->addAdjVertices(theVertices[31].get());
    theEdges[44]->addAdjVertices(theVertices[32].get());
    theEdges[44]->addAdjVertices(theVertices[33].get());
    theEdges[45]->addAdjVertices(theVertices[34].get());
    theEdges[45]->addAdjVertices(theVertices[35].get());
    for (int i = 46; i < 52; ++i) {
        theEdges[i]->addAdjVertices(theVertices[i-16].get());
        theEdges[i]->addAdjVertices(theVertices[i-10].get());
    }
    theEdges[52]->addAdjVertices(theVertices[37].get());
    theEdges[52]->addAdjVertices(theVertices[38].get());
    theEdges[53]->addAdjVertices(theVertices[39].get());
    theEdges[53]->addAdjVertices(theVertices[40].get());
    for (int i = 54; i < 60; ++i) {
        theEdges[i]->addAdjVertices(theVertices[i-18].get());
        theEdges[i]->addAdjVertices(theVertices[i-12].get());
    }
    theEdges[60]->addAdjVertices(theVertices[42].get());
    theEdges[60]->addAdjVertices(theVertices[43].get());
    theEdges[61]->addAdjVertices(theVertices[44].get());
    theEdges[61]->addAdjVertices(theVertices[45].get());
    theEdges[62]->addAdjVertices(theVertices[46].get());
    theEdges[62]->addAdjVertices(theVertices[47].get());
    for (int i = 63; i < 67; ++i) {
        theEdges[i]->addAdjVertices(theVertices[i-20].get());
        theEdges[i]->addAdjVertices(theVertices[i-15].get());
    }
    theEdges[67]->addAdjVertices(theVertices[48].get());
    theEdges[67]->addAdjVertices(theVertices[49].get());
    theEdges[68]->addAdjVertices(theVertices[50].get());
    theEdges[68]->addAdjVertices(theVertices[51].get());
    theEdges[69]->addAdjVertices(theVertices[49].get());
    theEdges[69]->addAdjVertices(theVertices[52].get());
    theEdges[70]->addAdjVertices(theVertices[50].get());
    theEdges[70]->addAdjVertices(theVertices[53].get());
    theEdges[71]->addAdjVertices(theVertices[52].get());
    theEdges[71]->addAdjVertices(theVertices[53].get());

	// connecting edges with edges
        theEdges[0]->addAdjEdges(theEdges[1].get());
        theEdges[0]->addAdjEdges(theEdges[2].get());
        theEdges[1]->addAdjEdges(theEdges[0].get());
        theEdges[1]->addAdjEdges(theEdges[3].get());
        theEdges[1]->addAdjEdges(theEdges[6].get());
        theEdges[2]->addAdjEdges(theEdges[0].get());
        theEdges[2]->addAdjEdges(theEdges[4].get());
        theEdges[2]->addAdjEdges(theEdges[7].get());
        theEdges[3]->addAdjEdges(theEdges[1].get());
        theEdges[3]->addAdjEdges(theEdges[5].get());
        theEdges[3]->addAdjEdges(theEdges[6].get());
        theEdges[4]->addAdjEdges(theEdges[2].get());
        theEdges[4]->addAdjEdges(theEdges[7].get());
        theEdges[4]->addAdjEdges(theEdges[8].get());
        theEdges[5]->addAdjEdges(theEdges[3].get());
        theEdges[5]->addAdjEdges(theEdges[9].get());
        theEdges[5]->addAdjEdges(theEdges[13].get());
        theEdges[6]->addAdjEdges(theEdges[1].get());
        theEdges[6]->addAdjEdges(theEdges[3].get());
        theEdges[6]->addAdjEdges(theEdges[10].get());
        theEdges[6]->addAdjEdges(theEdges[14].get());
        theEdges[7]->addAdjEdges(theEdges[2].get());
        theEdges[7]->addAdjEdges(theEdges[4].get());
        theEdges[7]->addAdjEdges(theEdges[10].get());
        theEdges[7]->addAdjEdges(theEdges[15].get());
        theEdges[8]->addAdjEdges(theEdges[4].get());
        theEdges[8]->addAdjEdges(theEdges[11].get());
        theEdges[8]->addAdjEdges(theEdges[16].get());
        theEdges[9]->addAdjEdges(theEdges[5].get());
        theEdges[9]->addAdjEdges(theEdges[12].get());
        theEdges[9]->addAdjEdges(theEdges[13].get());
        theEdges[10]->addAdjEdges(theEdges[6].get());
        theEdges[10]->addAdjEdges(theEdges[7].get());
        theEdges[10]->addAdjEdges(theEdges[14].get());
        theEdges[10]->addAdjEdges(theEdges[15].get());
        theEdges[11]->addAdjEdges(theEdges[8].get());
        theEdges[11]->addAdjEdges(theEdges[16].get());
        theEdges[11]->addAdjEdges(theEdges[17].get());
        theEdges[12]->addAdjEdges(theEdges[9].get());
        theEdges[12]->addAdjEdges(theEdges[20].get());
        theEdges[13]->addAdjEdges(theEdges[5].get());
        theEdges[13]->addAdjEdges(theEdges[9].get());
        theEdges[13]->addAdjEdges(theEdges[18].get());
        theEdges[13]->addAdjEdges(theEdges[21].get());
        theEdges[14]->addAdjEdges(theEdges[6].get());
        theEdges[14]->addAdjEdges(theEdges[10].get());
        theEdges[14]->addAdjEdges(theEdges[18].get());
        theEdges[14]->addAdjEdges(theEdges[22].get());
        theEdges[15]->addAdjEdges(theEdges[7].get());
        theEdges[15]->addAdjEdges(theEdges[10].get());
        theEdges[15]->addAdjEdges(theEdges[19].get());
        theEdges[15]->addAdjEdges(theEdges[23].get());
        theEdges[16]->addAdjEdges(theEdges[8].get());
        theEdges[16]->addAdjEdges(theEdges[11].get());
        theEdges[16]->addAdjEdges(theEdges[19].get());
        theEdges[16]->addAdjEdges(theEdges[24].get());
        theEdges[17]->addAdjEdges(theEdges[11].get());
        theEdges[17]->addAdjEdges(theEdges[25].get());
        theEdges[18]->addAdjEdges(theEdges[13].get());
        theEdges[18]->addAdjEdges(theEdges[14].get());
        theEdges[18]->addAdjEdges(theEdges[21].get());
        theEdges[18]->addAdjEdges(theEdges[22].get());
        theEdges[19]->addAdjEdges(theEdges[15].get());
        theEdges[19]->addAdjEdges(theEdges[16].get());
        theEdges[19]->addAdjEdges(theEdges[23].get());
        theEdges[19]->addAdjEdges(theEdges[24].get());
        theEdges[20]->addAdjEdges(theEdges[12].get());
        theEdges[20]->addAdjEdges(theEdges[26].get());
        theEdges[20]->addAdjEdges(theEdges[29].get());
        theEdges[21]->addAdjEdges(theEdges[13].get());
        theEdges[21]->addAdjEdges(theEdges[18].get());
        theEdges[21]->addAdjEdges(theEdges[26].get());
        theEdges[21]->addAdjEdges(theEdges[30].get());
        theEdges[22]->addAdjEdges(theEdges[14].get());
        theEdges[22]->addAdjEdges(theEdges[18].get());
        theEdges[22]->addAdjEdges(theEdges[27].get());
        theEdges[22]->addAdjEdges(theEdges[31].get());
        theEdges[23]->addAdjEdges(theEdges[15].get());
        theEdges[23]->addAdjEdges(theEdges[19].get());
        theEdges[23]->addAdjEdges(theEdges[27].get());
        theEdges[23]->addAdjEdges(theEdges[32].get());
        theEdges[24]->addAdjEdges(theEdges[16].get());
        theEdges[24]->addAdjEdges(theEdges[19].get());
        theEdges[24]->addAdjEdges(theEdges[33].get());
        theEdges[24]->addAdjEdges(theEdges[28].get());
        theEdges[25]->addAdjEdges(theEdges[17].get());
        theEdges[25]->addAdjEdges(theEdges[34].get());
        theEdges[25]->addAdjEdges(theEdges[28].get());
        theEdges[26]->addAdjEdges(theEdges[20].get());
        theEdges[26]->addAdjEdges(theEdges[21].get());
        theEdges[26]->addAdjEdges(theEdges[39].get());
        theEdges[26]->addAdjEdges(theEdges[30].get());
        theEdges[27]->addAdjEdges(theEdges[22].get());
        theEdges[27]->addAdjEdges(theEdges[23].get());
        theEdges[27]->addAdjEdges(theEdges[31].get());
        theEdges[27]->addAdjEdges(theEdges[32].get());
        theEdges[28]->addAdjEdges(theEdges[24].get());
        theEdges[28]->addAdjEdges(theEdges[25].get());
        theEdges[28]->addAdjEdges(theEdges[33].get());
        theEdges[28]->addAdjEdges(theEdges[34].get());
        theEdges[29]->addAdjEdges(theEdges[20].get());
        theEdges[29]->addAdjEdges(theEdges[26].get());
        theEdges[29]->addAdjEdges(theEdges[37].get());
        theEdges[30]->addAdjEdges(theEdges[21].get());
        theEdges[30]->addAdjEdges(theEdges[26].get());
        theEdges[30]->addAdjEdges(theEdges[38].get());
        theEdges[30]->addAdjEdges(theEdges[35].get());
        theEdges[31]->addAdjEdges(theEdges[22].get());
        theEdges[31]->addAdjEdges(theEdges[27].get());
        theEdges[31]->addAdjEdges(theEdges[32].get());
        theEdges[31]->addAdjEdges(theEdges[39].get());
        theEdges[32]->addAdjEdges(theEdges[23].get());
        theEdges[32]->addAdjEdges(theEdges[27].get());
        theEdges[32]->addAdjEdges(theEdges[36].get());
        theEdges[32]->addAdjEdges(theEdges[40].get());
        theEdges[33]->addAdjEdges(theEdges[24].get());
        theEdges[33]->addAdjEdges(theEdges[28].get());
        theEdges[33]->addAdjEdges(theEdges[36].get());
        theEdges[33]->addAdjEdges(theEdges[41].get());
        theEdges[34]->addAdjEdges(theEdges[25].get());
        theEdges[34]->addAdjEdges(theEdges[28].get());
        theEdges[34]->addAdjEdges(theEdges[42].get());
        theEdges[35]->addAdjEdges(theEdges[30].get());
        theEdges[35]->addAdjEdges(theEdges[31].get());
        theEdges[35]->addAdjEdges(theEdges[38].get());
        theEdges[35]->addAdjEdges(theEdges[39].get());
        theEdges[36]->addAdjEdges(theEdges[32].get());
        theEdges[36]->addAdjEdges(theEdges[33].get());
        theEdges[36]->addAdjEdges(theEdges[40].get());
        theEdges[36]->addAdjEdges(theEdges[41].get());
        theEdges[37]->addAdjEdges(theEdges[29].get());
        theEdges[37]->addAdjEdges(theEdges[43].get());
        theEdges[37]->addAdjEdges(theEdges[46].get());
        theEdges[38]->addAdjEdges(theEdges[30].get());
        theEdges[38]->addAdjEdges(theEdges[35].get());
        theEdges[38]->addAdjEdges(theEdges[43].get());
        theEdges[38]->addAdjEdges(theEdges[47].get());
        theEdges[39]->addAdjEdges(theEdges[31].get());
        theEdges[39]->addAdjEdges(theEdges[35].get());
        theEdges[39]->addAdjEdges(theEdges[44].get());
        theEdges[39]->addAdjEdges(theEdges[48].get());
        theEdges[40]->addAdjEdges(theEdges[32].get());
        theEdges[40]->addAdjEdges(theEdges[44].get());
        theEdges[40]->addAdjEdges(theEdges[49].get());
        theEdges[41]->addAdjEdges(theEdges[33].get());
        theEdges[41]->addAdjEdges(theEdges[36].get());
        theEdges[41]->addAdjEdges(theEdges[41].get());
        theEdges[41]->addAdjEdges(theEdges[45].get());
        theEdges[42]->addAdjEdges(theEdges[34].get());
        theEdges[42]->addAdjEdges(theEdges[45].get());
        theEdges[42]->addAdjEdges(theEdges[51].get());
        theEdges[43]->addAdjEdges(theEdges[37].get());
        theEdges[43]->addAdjEdges(theEdges[38].get());
        theEdges[43]->addAdjEdges(theEdges[46].get());
        theEdges[43]->addAdjEdges(theEdges[47].get());
        theEdges[44]->addAdjEdges(theEdges[39].get());
        theEdges[44]->addAdjEdges(theEdges[40].get());
        theEdges[44]->addAdjEdges(theEdges[48].get());
        theEdges[44]->addAdjEdges(theEdges[49].get());
        theEdges[45]->addAdjEdges(theEdges[41].get());
        theEdges[45]->addAdjEdges(theEdges[42].get());
        theEdges[45]->addAdjEdges(theEdges[50].get());
        theEdges[45]->addAdjEdges(theEdges[51].get());
        theEdges[46]->addAdjEdges(theEdges[37].get());
        theEdges[46]->addAdjEdges(theEdges[43].get());
        theEdges[46]->addAdjEdges(theEdges[54].get());
        theEdges[47]->addAdjEdges(theEdges[38].get());
        theEdges[47]->addAdjEdges(theEdges[43].get());
        theEdges[47]->addAdjEdges(theEdges[52].get());
        theEdges[47]->addAdjEdges(theEdges[55].get());
        theEdges[48]->addAdjEdges(theEdges[39].get());
        theEdges[48]->addAdjEdges(theEdges[44].get());
        theEdges[48]->addAdjEdges(theEdges[52].get());
        theEdges[48]->addAdjEdges(theEdges[56].get());
        theEdges[49]->addAdjEdges(theEdges[40].get());
        theEdges[49]->addAdjEdges(theEdges[44].get());
        theEdges[49]->addAdjEdges(theEdges[53].get());
        theEdges[49]->addAdjEdges(theEdges[57].get());
        theEdges[50]->addAdjEdges(theEdges[41].get());
        theEdges[50]->addAdjEdges(theEdges[45].get());
        theEdges[50]->addAdjEdges(theEdges[50].get());
        theEdges[50]->addAdjEdges(theEdges[53].get());
        theEdges[51]->addAdjEdges(theEdges[42].get());
        theEdges[51]->addAdjEdges(theEdges[45].get());
        theEdges[51]->addAdjEdges(theEdges[59].get());
        theEdges[52]->addAdjEdges(theEdges[47].get());
        theEdges[52]->addAdjEdges(theEdges[48].get());
        theEdges[52]->addAdjEdges(theEdges[55].get());
        theEdges[52]->addAdjEdges(theEdges[56].get());
        theEdges[53]->addAdjEdges(theEdges[49].get());
        theEdges[53]->addAdjEdges(theEdges[50].get());
        theEdges[53]->addAdjEdges(theEdges[57].get());
        theEdges[53]->addAdjEdges(theEdges[58].get());
        theEdges[54]->addAdjEdges(theEdges[46].get());
        theEdges[54]->addAdjEdges(theEdges[60].get());
        theEdges[55]->addAdjEdges(theEdges[47].get());
        theEdges[55]->addAdjEdges(theEdges[52].get());
        theEdges[55]->addAdjEdges(theEdges[60].get());
        theEdges[55]->addAdjEdges(theEdges[63].get());
        theEdges[56]->addAdjEdges(theEdges[48].get());
        theEdges[56]->addAdjEdges(theEdges[52].get());
        theEdges[56]->addAdjEdges(theEdges[61].get());
        theEdges[56]->addAdjEdges(theEdges[64].get());
        theEdges[57]->addAdjEdges(theEdges[49].get());
        theEdges[57]->addAdjEdges(theEdges[53].get());
        theEdges[57]->addAdjEdges(theEdges[61].get());
        theEdges[57]->addAdjEdges(theEdges[65].get());
        theEdges[58]->addAdjEdges(theEdges[50].get());
        theEdges[58]->addAdjEdges(theEdges[53].get());
        theEdges[58]->addAdjEdges(theEdges[62].get());
        theEdges[58]->addAdjEdges(theEdges[66].get());
        theEdges[59]->addAdjEdges(theEdges[51].get());
        theEdges[59]->addAdjEdges(theEdges[62].get());
        theEdges[60]->addAdjEdges(theEdges[54].get());
        theEdges[60]->addAdjEdges(theEdges[55].get());
        theEdges[60]->addAdjEdges(theEdges[63].get());
        theEdges[61]->addAdjEdges(theEdges[56].get());
        theEdges[61]->addAdjEdges(theEdges[57].get());
        theEdges[61]->addAdjEdges(theEdges[64].get());
        theEdges[61]->addAdjEdges(theEdges[65].get());
        theEdges[62]->addAdjEdges(theEdges[58].get());
        theEdges[62]->addAdjEdges(theEdges[59].get());
        theEdges[62]->addAdjEdges(theEdges[66].get());
        theEdges[63]->addAdjEdges(theEdges[55].get());
        theEdges[63]->addAdjEdges(theEdges[60].get());
        theEdges[63]->addAdjEdges(theEdges[67].get());
        theEdges[64]->addAdjEdges(theEdges[56].get());
        theEdges[64]->addAdjEdges(theEdges[61].get());
        theEdges[64]->addAdjEdges(theEdges[67].get());
        theEdges[64]->addAdjEdges(theEdges[69].get());
        theEdges[65]->addAdjEdges(theEdges[57].get());
        theEdges[65]->addAdjEdges(theEdges[61].get());
        theEdges[65]->addAdjEdges(theEdges[68].get());
        theEdges[65]->addAdjEdges(theEdges[70].get());
        theEdges[66]->addAdjEdges(theEdges[58].get());
        theEdges[66]->addAdjEdges(theEdges[62].get());
        theEdges[66]->addAdjEdges(theEdges[68].get());
        theEdges[67]->addAdjEdges(theEdges[63].get());
        theEdges[67]->addAdjEdges(theEdges[64].get());
        theEdges[67]->addAdjEdges(theEdges[69].get());
        theEdges[68]->addAdjEdges(theEdges[65].get());
        theEdges[68]->addAdjEdges(theEdges[66].get());
        theEdges[68]->addAdjEdges(theEdges[70].get());
        theEdges[69]->addAdjEdges(theEdges[64].get());
        theEdges[69]->addAdjEdges(theEdges[67].get());
        theEdges[69]->addAdjEdges(theEdges[71].get());
        theEdges[70]->addAdjEdges(theEdges[65].get());
        theEdges[70]->addAdjEdges(theEdges[68].get());
        theEdges[70]->addAdjEdges(theEdges[71].get());
        theEdges[71]->addAdjEdges(theEdges[69].get());
        theEdges[71]->addAdjEdges(theEdges[70].get());

    // adding vertices to tiles
    for (int i = 0; i < 19; ++i) {
        if (i == 0) {
            theTiles[i]->attach(theVertices[0]);
            theTiles[i]->attach(theVertices[1]);
            theTiles[i]->attach(theVertices[3]);
            theTiles[i]->attach(theVertices[4]);
            theTiles[i]->attach(theVertices[8]);
            theTiles[i]->attach(theVertices[9]);
        } else if (i == 1) {
            theTiles[i]->attach(theVertices[2]);
            theTiles[i]->attach(theVertices[3]);
            theTiles[i]->attach(theVertices[7]);
            theTiles[i]->attach(theVertices[8]);
            theTiles[i]->attach(theVertices[13]);
            theTiles[i]->attach(theVertices[14]);
        } else if (i == 2) {
            theTiles[i]->attach(theVertices[4]);
            theTiles[i]->attach(theVertices[5]);
            theTiles[i]->attach(theVertices[9]);
            theTiles[i]->attach(theVertices[10]);
            theTiles[i]->attach(theVertices[15]);
            theTiles[i]->attach(theVertices[16]);
        } else if (i == 3) {
            theTiles[i]->attach(theVertices[6]);
            theTiles[i]->attach(theVertices[7]);
            theTiles[i]->attach(theVertices[12]);
            theTiles[i]->attach(theVertices[13]);
            theTiles[i]->attach(theVertices[18]);
            theTiles[i]->attach(theVertices[19]);
        } else if (i == 4) {
            theTiles[i]->attach(theVertices[8]);
            theTiles[i]->attach(theVertices[9]);
            theTiles[i]->attach(theVertices[14]);
            theTiles[i]->attach(theVertices[15]);
            theTiles[i]->attach(theVertices[20]);
            theTiles[i]->attach(theVertices[21]);
        } else if (i == 5) {
            theTiles[i]->attach(theVertices[10]);
            theTiles[i]->attach(theVertices[11]);
            theTiles[i]->attach(theVertices[16]);
            theTiles[i]->attach(theVertices[17]);
            theTiles[i]->attach(theVertices[22]);
            theTiles[i]->attach(theVertices[23]);
        } else if (i == 6) {
            theTiles[i]->attach(theVertices[13]);
            theTiles[i]->attach(theVertices[14]);
            theTiles[i]->attach(theVertices[19]);
            theTiles[i]->attach(theVertices[20]);
            theTiles[i]->attach(theVertices[25]);
            theTiles[i]->attach(theVertices[26]);
        } else if (i == 7) {
            theTiles[i]->attach(theVertices[15]);
            theTiles[i]->attach(theVertices[16]);
            theTiles[i]->attach(theVertices[21]);
            theTiles[i]->attach(theVertices[22]);
            theTiles[i]->attach(theVertices[27]);
            theTiles[i]->attach(theVertices[28]);
        } else if (i == 8) {
            theTiles[i]->attach(theVertices[18]);
            theTiles[i]->attach(theVertices[19]);
            theTiles[i]->attach(theVertices[24]);
            theTiles[i]->attach(theVertices[25]);
            theTiles[i]->attach(theVertices[30]);
            theTiles[i]->attach(theVertices[31]);
        } else if (i == 9) {
            theTiles[i]->attach(theVertices[20]);
            theTiles[i]->attach(theVertices[21]);
            theTiles[i]->attach(theVertices[26]);
            theTiles[i]->attach(theVertices[27]);
            theTiles[i]->attach(theVertices[32]);
            theTiles[i]->attach(theVertices[33]);
        } else if (i == 10) {
            theTiles[i]->attach(theVertices[22]);
            theTiles[i]->attach(theVertices[23]);
            theTiles[i]->attach(theVertices[28]);
            theTiles[i]->attach(theVertices[29]);
            theTiles[i]->attach(theVertices[34]);
            theTiles[i]->attach(theVertices[35]);
        } else if (i == 11) {
            theTiles[i]->attach(theVertices[25]);
            theTiles[i]->attach(theVertices[26]);
            theTiles[i]->attach(theVertices[31]);
            theTiles[i]->attach(theVertices[32]);
            theTiles[i]->attach(theVertices[37]);
            theTiles[i]->attach(theVertices[38]);
        } else if (i == 12) {
            theTiles[i]->attach(theVertices[27]);
            theTiles[i]->attach(theVertices[28]);
            theTiles[i]->attach(theVertices[33]);
            theTiles[i]->attach(theVertices[34]);
            theTiles[i]->attach(theVertices[39]);
            theTiles[i]->attach(theVertices[40]);
        } else if (i == 13) {
            theTiles[i]->attach(theVertices[30]);
            theTiles[i]->attach(theVertices[31]);
            theTiles[i]->attach(theVertices[36]);
            theTiles[i]->attach(theVertices[37]);
            theTiles[i]->attach(theVertices[42]);
            theTiles[i]->attach(theVertices[43]);
        } else if (i == 14) {
            theTiles[i]->attach(theVertices[32]);
            theTiles[i]->attach(theVertices[33]);
            theTiles[i]->attach(theVertices[38]);
            theTiles[i]->attach(theVertices[39]);
            theTiles[i]->attach(theVertices[44]);
            theTiles[i]->attach(theVertices[45]);
        } else if (i == 15) {
            theTiles[i]->attach(theVertices[34]);
            theTiles[i]->attach(theVertices[35]);
            theTiles[i]->attach(theVertices[40]);
            theTiles[i]->attach(theVertices[41]);
            theTiles[i]->attach(theVertices[46]);
            theTiles[i]->attach(theVertices[47]);
        } else if (i == 16) {
            theTiles[i]->attach(theVertices[37]);
            theTiles[i]->attach(theVertices[38]);
            theTiles[i]->attach(theVertices[43]);
            theTiles[i]->attach(theVertices[44]);
            theTiles[i]->attach(theVertices[48]);
            theTiles[i]->attach(theVertices[49]);
        } else if (i == 17) {
            theTiles[i]->attach(theVertices[39]);
            theTiles[i]->attach(theVertices[40]);
            theTiles[i]->attach(theVertices[45]);
            theTiles[i]->attach(theVertices[46]);
            theTiles[i]->attach(theVertices[50]);
            theTiles[i]->attach(theVertices[51]);
        } else if (i == 18) {
            theTiles[i]->attach(theVertices[44]);
            theTiles[i]->attach(theVertices[45]);
            theTiles[i]->attach(theVertices[49]);
            theTiles[i]->attach(theVertices[50]);
            theTiles[i]->attach(theVertices[52]);
            theTiles[i]->attach(theVertices[53]);
        }
    }
}
