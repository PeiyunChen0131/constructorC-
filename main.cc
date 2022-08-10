#include <memory>
#include <string>
#include <chrono>
#include <iostream>
#include <fstream>
#include "controller.h"

int main( int argc, char * argv[] ) {
    const std::string SEED_ARG = "-seed";
    const std::string LOAD_ARG = "-load";
    const std::string BOARD_ARG = "-board";
    const std::string RANDOM_ARG = "-random-board";
    
    auto controller = std::make_unique<Controller>();
    std::cin.exceptions(std::ios::failbit);

    bool isLoad = false;
    bool isBoard = false;
    bool isRandom = false;
    bool hasSeed = false;
    std::string fname = "layout.txt";
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

    try {
        for (int i = 1; i < argc; ++i) {
            if (argv[i] == LOAD_ARG) {
                if (isLoad) {
                    throw std::invalid_argument( "ERROR: already specified -load once before" );
                } else if (isBoard) {
                    throw std::invalid_argument( "ERROR: already specified -board, can't also specify -load" );
                } else {
                    if (isRandom) {
                        std::cerr << "ERROR: previous -random-board being ignored in favour of -load" << std::endl;
                        isRandom = false;
                    }
                    i++;
                    if (i >= argc) {
                        throw std::invalid_argument( "ERROR: -load missing filename argument" );
                    }
                    fname = std::string{ argv[i] };
                    isLoad = true;
                }
            } else if (argv[i] == BOARD_ARG) {
                if (isLoad) {
                    throw std::invalid_argument( "ERROR: already specified -load, can't also specify -board" );
                } else if (isBoard) {
                    throw std::invalid_argument( "ERROR: already specified -board once before" );
                } else {
                    if (isRandom) {
                        std::cerr << "ERROR: previous -random-board being ignored in favour of -board" << std::endl;
                        isRandom = false;
                    }
                    i++;
                    if (i >= argc) {
                        throw std::invalid_argument( "ERROR: -board missing filename argument" );
                    }
                    fname = std::string{ argv[i] };
                    isBoard = true;
                }
            } else if (argv[i] == RANDOM_ARG) {
                if (isLoad) {
                    std::cerr << "ERROR: -random-board being ignored in favour of previous -load" << std::endl;
                } else if (isBoard) {
                    std::cerr << "ERROR: -random-board being ignored in favour of previous -board" << std::endl;
                } else {
                    isRandom = true;
                }
            } else if (argv[i] == SEED_ARG) {
                if (hasSeed) {
                    throw std::invalid_argument( "ERROR: already specified -seed once before" );
                } else {
                    i++;
                    if (i >= argc) {
                        throw std::invalid_argument( "ERROR: -seed missing seed argument" );
                    }

                    try {
                        seed = std::stoi( std::string{ argv[i] } );
                    } catch( std::invalid_argument & e ) {
                            std::cerr << e.what() << std::endl;
                            return 1;
                    } catch( std::out_of_range & e ) {
                            std::cerr << e.what() << std::endl;
                            return -1;
                    }
                    hasSeed = true;
                }
            } else {
                std::string str = "ERROR: unrecognized argument ";
                str.append(argv[i]);
                throw std::invalid_argument( str );
            }
        }
    } catch ( std::invalid_argument & e ) {
        std::cerr << e.what() << std::endl;
        return 255;
    }

    int type = 2;
    if (isLoad) {
        type = 0;
    } else if (isRandom) {
        type = 1;
    } else if (isBoard) {
        type = 2;
    } else {
        type = 2;
    }

    try {
        controller->cmdCreateBoard(seed, fname, type);
    } catch (std::ifstream::failure &readfail) {
        std::cerr << "ERROR: Unable to open file " << fname << " for board layout." << std::endl;
        return 1;
    } catch (std::invalid_argument & e) {
	std::cerr << "ERROR: Unable to open file " << fname << " for board layout." << std::endl;
        return 1;
    }

    bool playAgain = false;
    do {
        try{
	    if (!isLoad) {
            	controller->cmdStartOfGame();
	    }
	    isLoad = false;
        } catch (std::ios::failure &readfail) {
            std::cerr << "EOF reached" << std::endl;
            return 0;
        }

        try{
            playAgain = controller->playGame();
        } catch (std::ios::failure &readfail) {
            controller->cmdSave();
            return 0;
            // save to backup.sv
        }

        if (playAgain) {
            seed = std::chrono::system_clock::now().time_since_epoch().count();
            type = 2;
            controller->cmdCreateBoard(seed, fname, type);
        }
    } while (playAgain);

    return 0;
}
