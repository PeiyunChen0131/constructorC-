#ifndef _BOARD_H_
#define _BOARD_H_
#include <memory>
#include <vector>
#include <set>
#include <string>
#include "tile.h"
#include "edge.h"
#include "vertex.h"
#include "player.h"

class Board {
	protected:
	std::vector<std::shared_ptr<Tile>> theTiles;
    	std::vector<std::shared_ptr<Edge>> theEdges;
    	std::vector<std::shared_ptr<Vertex>> theVertices;
    	std::vector<std::shared_ptr<Player>> thePlayer;
    	int howManyPlayer = 4;
	int geeseIsAt = -1;
	int whosTurn = 0;

	public:
	bool placeGeese(int geeseAtTile);

	std::string getPlayerStatus(int num);

	int getWhosTurn();

	void printPlayerRes(int num);

	bool checkPts(int num);

	void boardLoseResources();

	std::set<std::string> boardWhoToSteal();
	std::string getPlayerNameAt(int n);

	void boardSteal(std::string colour, int giveTo);

	//Distribute Resources After Rolled Dice
   	//override for bigger resources
	void distributeResources(int tileValue);

	//DURING THE TURN
    	// valid output:
    	// returns: "Success"  "CanNotBuild"   "NotEnoughResource"
    	// valid input:
    	// resType: "Basement"  "House"  "Tower"   "Road"
        // stage: "FORCEBUILD"  "STARTOFGAME" "DURINGTURN"
	// EXAMPLE: std::string output = theBoard->boardBuildRes(whosTurn,resNum,"Basement","DURINGTURN");
    	std::string boardBuildRes(int whosTurn, int resNum, std::string resType, std::string stage);

	std::string boardImprove(int turn, int resNum);

	int colourToInt(std::string colour);

	std::string intToColour(int input);

	bool boardCheckTrade(int playerNum, std::string resource);

	void boardTrade(int whosTurn,int toPlayer,std::string give,std::string take);

	void boardSave(std::string outFile, int whosTurn);

	void boardSetDice(int turn, std::string diceType);

	int boardRollDice(int turn);

	void printTopEdge(int *vertice, int *edge);

	void printBoard();

	void initBoard();

	virtual void createBoard(unsigned seed, std::string fname) = 0;

	int playerNum();

};

#endif
