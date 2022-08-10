#include <memory>
#include <iostream>
#include "controller.h"
#include "loadlevel.h"
#include "randomlevel.h"
#include "layoutlevel.h"
#include "level.h"

void Controller::cmdCreateBoard(unsigned seed, std::string fname, int type){
    std::shared_ptr<Level> factoryBoard;
    if (type == 0) {
        factoryBoard = std::make_shared<LoadLevel>();
    } else if (type == 1) {
        factoryBoard = std::make_shared<RandomLevel>();
    } else {
        factoryBoard = std::make_shared<LayoutLevel>();
    }
    theBoard = factoryBoard->getBoard(seed, fname);
}

void Controller::cmdStartOfGame() {
    int resNum = 0;
    std::string message;

    theBoard->printBoard();
    for(int i = 0; i < theBoard->playerNum(); i++) {
        do {
	    std::cout << "Builder " << theBoard->intToColour(i) << " where do you want to build a basement?" << std::endl;
            try {
                std::cin >> resNum;
                message = theBoard->boardBuildRes(i, resNum, "Basement", "STARTOFGAME");
                if (message != "Success") {
                    std::cerr << "You cannot build here." << std::endl;
                }
            } catch (std::ios::failure &error) {
                if (std::cin.eof()) {
                    throw error;
                } else {
                    std::cin.clear();
                    std::cin.ignore();
                    std::cout << "ERROR: is not a valid integer" << std::endl;
                }
            }
        } while (message != "Success");
    }

    for(int i = theBoard->playerNum()-1; i >= 0; i--) {
        do {
	    std::cout << "Builder " << theBoard->intToColour(i) << " where do you want to build a basement?" << std::endl;
            try {
                std::cin >> resNum;
                message = theBoard->boardBuildRes(i, resNum, "Basement", "STARTOFGAME");
                if (message != "Success") {
                    std::cerr << "You cannot build here." << std::endl;
                }
            } catch (std::ios::failure &error) {
                if (std::cin.eof()) {
                    throw error;
                } else {
                    std::cin.clear();
                    std::cin.ignore();
                    std::cout << "ERROR: is not a valid integer" << std::endl;
                }
            }
        } while (message != "Success");
    }
}

void Controller::cmdSave() {
    theBoard->boardSave("backup.sv", whosTurn);
    std::cout << "Saving to backup.sv..." << std::endl;
}

bool Controller::playGame() {
	whosTurn = theBoard->getWhosTurn();
        while(true) {
        theBoard->printBoard();
	std::cout << "Builder " << theBoard->intToColour(whosTurn) << "'s turn." << std::endl;
        
	// START OF TURN
        int diceNum = 0;
        while (true) {
            std::string diceCommand;
            std::cout << ">";
            std::cin >> diceCommand;
            if (diceCommand == "load") {
                theBoard->boardSetDice(whosTurn, "load");
            } else if (diceCommand == "fair") {
                theBoard->boardSetDice(whosTurn, "fair");
            } else if (diceCommand == "roll")  {
                diceNum = theBoard->boardRollDice(whosTurn);
                //std::cout << "The number you rolled is " << diceNum << std::endl;
                break;
            } else if (diceCommand == "status") {
                int playerNum = theBoard->playerNum();
                std::string playerName;
                for (int i = 0; i < playerNum; ++i) {
                    playerName = theBoard->intToColour(i);
                    std::cout << playerName << " has " << theBoard->getPlayerStatus(i) << std::endl;
                }
            } else if (diceCommand == "help") {
                std::cout << "Valid commands:" << std::endl;
                std::cout << "~ load : changes current builder's dice type to 'loaded'" << std::endl;
                std::cout << "~ fair : changes current builder's dice type to 'fair'" << std::endl;
                std::cout << "~ roll : rolls the dice and distributes resources." << std::endl;
                std::cout << "~ status : prints the current status of all builders in order from builder 0 to 3." << std::endl;
                std::cout << "~ help : prints out the list of commands." << std::endl;
            } else {
                std::cout << "Invalid command" << std::endl;
                std::cout << "Please enter 'help' for a list of valid commands." << std::endl;
            }
        }
        //std::cout << "rolled" << std::endl;
        if (diceNum == 7) {
            int placeGeeseAt = 0;
	        theBoard->boardLoseResources();
            while(true) {
                // dbc exception safe: if rolled 7 then save
                try {
                    std::cout << "Choose where to place the GEESE," << std::endl;
                    std::cout << ">";
                    std::cin >> placeGeeseAt;
                    bool success = false;
                    success = theBoard->placeGeese(placeGeeseAt);
                    if (success) {
                        break;
                    } else {
                        std::cout << "You can't place geese here" << std::endl;
                    }
                // dbc try-catch here
                } catch (std::ios::failure &error) {
                    if (std::cin.eof()) {
                        throw error;
                    } else {
                        std::cin.clear();
                        std::cin.ignore();
                        std::cout << "ERROR: isn't a valid integer" << std::endl;
                    }
                }
            } //while
            // all player loses half if >10 resources
            // choose whom to steal
            std::set<std::string> setOfPlayer = theBoard->boardWhoToSteal();
            if (setOfPlayer.empty()) {
                std::cout << "Builder " << theBoard->getPlayerNameAt(whosTurn) << " has no builders to steal from" << std::endl;
            } else {
                std::cout << "Choose a builder to steal from" << std::endl;
                for (auto n : setOfPlayer) {
                    std::cout << n << " ";
                }
                std::cout << std::endl;
                while(true) {
                    //std::cout << "Choose a builder to steal from.";
                    // steal
                    std::string input;
                    std::cout << ">";
                    std::cin >> input;
                    auto choose = setOfPlayer.find(input);
                    if (choose != setOfPlayer.end()) {
                        theBoard->boardSteal(input,whosTurn);
                        break;
                    } else {
                        // output what?
                        std::cout << "Invalid player" << std::endl;
                    }
                }
            }
        } else {
            theBoard->distributeResources(diceNum);
        }
//std::cout << "here" << std::endl;
        // DURING THE TURN
        std::string input;
        // dbc try-catch here
	std::cout << "Enter a command:" << std::endl;
        std::cout << ">";
        while(std::cin >> input) {
            if (input == "board") {
                theBoard->printBoard();
            } else if (input == "status") {
                int playerNum = theBoard->playerNum();
                std::string playerName;
                for (int i = 0; i < playerNum; ++i) {
                    playerName = theBoard->intToColour(i);
                    std::cout << playerName << " has " << theBoard->getPlayerStatus(i) << std::endl;
                }
                //thePlayer//->status
            } else if (input == "residences") {
                std::cout << theBoard->intToColour(whosTurn) << " has built:" << std::endl;
                theBoard->printPlayerRes(whosTurn);
            } else if (input == "build-road") {
                try {
                    int edgeNum;
                    std::cout << ">";
                    std::cin >> edgeNum;
                    std::string message;
                    message = theBoard->boardBuildRes(whosTurn,edgeNum,"Road","DURINGTURN");
                    if (message == "Success") {
                        //std::cout << "Build Road" << std::endl;
                    } else if (message == "NotEnoughResource") {
                        std::cout << "You do not have enough resources" << std::endl;
                    } else {
                        std::cout << "You cannot build here" << std::endl;
                    }
                } catch (std::ios::failure &error) {
                    if (std::cin.eof()) {
                        throw error;
                    } else {
                        std::cin.clear();
                        std::cin.ignore();
                        std::cout << "ERROR: isn't a valid integer" << std::endl;
                    }
                }
            } else if (input == "build-res") {

                    try {
                        int resNum;
                        std::cout << ">";
                        std::cin >> resNum;
                        std::string message;
                        message = theBoard->boardBuildRes(whosTurn,resNum,"Basement","DURINGTURN");
                        if (message == "Success") {
                            std::cout << "builed residence" << std::endl;
                            break;
                        } else if (message == "NotEnoughResource") {
                            std::cout << "You do not have enough resources" << std::endl;
                        } else {
                            std::cout << "Player can't build here" << std::endl;
                        }
                    } catch (std::ios::failure &error) {
                        if (std::cin.eof()) {
                            throw error;
                        } else {
                            std::cin.clear();
                            std::cin.ignore();
                            std::cout << "ERROR: is not a valid integer" << std::endl;
                        }
                    }

            } else if (input == "trade") {
                int proposePlayer = 0;
                int toPlayer = 0;
                std::string give;
                std::string take;
                while (true) {
                    std::string iscolour;
                    std::cout << ">";
                    std::cin >> iscolour;
                    toPlayer = theBoard->colourToInt(iscolour);
                    if (toPlayer >= 0) {
                        break;
                    }
                    std::cout << "Invalid colour" << std::endl;
                }
                while (true) {
                        //dbc output for not enough resources here
                        std::cout << ">";
                    std::cin >> give;
                    bool canTrade = theBoard->boardCheckTrade(whosTurn,give);
                    if (canTrade) {
                        break;
                    }
                    std::cout << "You do not have enough resources" << std::endl;
                }
                while (true) {
                    //dbc output for not enough resources here
                    std::cout << ">";
                    std::cin >> take;
                    bool canTrade = theBoard->boardCheckTrade(toPlayer,take);
                    if (canTrade) {
                        break;
                    }
                    std::cout << "You do not have enough resources" << std::endl;
                }
                if (proposePlayer != toPlayer) {
                    while (true) {
                        std::cout << theBoard->intToColour(whosTurn) << " Offers ";
                        std::cout << theBoard->intToColour(toPlayer) << " one ";
                        std::cout << give << " for one " << take << "." << std::endl;
                        std::cout << "Does " << theBoard->intToColour(toPlayer);
                        std::cout << " accept this offer?" << std::endl;
                        
                        std::string tradeInput;
                        std::cout << ">";
                        std::cin >> tradeInput;
                        if (tradeInput == "Yes") {
                            theBoard->boardTrade(whosTurn, toPlayer, give, take);
                            break;
                        } else if (tradeInput == "No") {
                            break;
                        } else {
                            std::cout << "Invalid Input" << std::endl;
                        }
                    }
                    
                } else {
                    std::cout << "Can't trade with yourself" << std::endl;
                }
            } else if (input == "improve") {
                try {
                    int resNum = 0;
                    std::cout << ">";
                    std::cin >> resNum;
                    std::string message;
                    message = theBoard->boardImprove(whosTurn,resNum);
                    if (message == "Success") {
                        std::cout << "Improved residence" << std::endl;
                        
                    } else if (message == "NotEnoughResource") {
                        std::cout << "Player does not have enough resource" << std::endl;
                    } else {
                        std::cout << "Player can't build here" << std::endl;
                    }
                } catch (std::ios::failure &error) {
                    if (std::cin.eof()) {
                        throw error;
                    } else {
                        std::cin.clear();
                        std::cin.ignore();
                        std::cout << "ERROR: isn't a valid integer" << std::endl;
                    }
                }
            } else if (input == "save") {
                std::string outFile;
                std::cout << ">";
                std::cin >> outFile;
                theBoard->boardSave(outFile,whosTurn);
            } else if (input == "next") {
                break;
            } else if (input == "help") {
                std::cout << "Valid commands :" << std::endl;
                std::cout << "~ board : prints the current board." << std::endl;
                std::cout << "~ status : prints the current status of all builders in order from builder 0 to 3." << std::endl;
                std::cout << "~ residences : prints the residences the current builder has currently completed." << std::endl;
                std::cout << "~ build - road <road#> : attempts to builds the road at <road#>." << std::endl;
                std::cout << "~ build - res <housing#> : attempts to builds a basement at <housing#>." << std::endl;
                std::cout << "~ improve <housing#> : attempts to improve the residence at <housing#>." << std::endl;
                std::cout << "~ trade <colour> <give> <take> : attempts to trade with builder <colour>, giving one resource of type <give> and receiving one resource of type <take>." << std::endl;
                std::cout << "~ next : passes control onto the next builder in the game." << std::endl;
                std::cout << "~ save <file> : saves the current game state to <file>." << std::endl;
                std::cout << "~ help : prints out the list of commands." << std::endl;
            } else {
                std::cout << "Invalid command" << std::endl;
                std::cout << "Please enter 'help' for a list of valid commands." << std::endl;
            }

	    std::cout << "Enter a command:" << std::endl;
        }

        std::string again;
        if (theBoard->checkPts(whosTurn)) {
            std::cout << "Congratulations!! " << theBoard->intToColour(whosTurn) << "wins!!" << std::endl;
            bool invalidInput = false;
            do {
                std::cout << "Would you like to play again? yes/no" << std::endl;
                std::cin >> again;
                if (again == "yes") {
                    return true;
                } else if (again == "no") {
                    return false;
                } else {
                    std::cout << "Error: Invalid command" << std::endl;
                    invalidInput = true;
                }
            } while (invalidInput);
        }

        whosTurn += 1;
        if (whosTurn >= theBoard->playerNum()) {
            whosTurn = 0;
        }
    }
}
