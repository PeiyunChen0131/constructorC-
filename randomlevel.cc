#include <memory>
#include "randomlevel.h"
#include "randomboard.h"

std::shared_ptr<Board> RandomLevel::getBoard(unsigned seed, std::string fname) {
    std::shared_ptr<Board> theBoard = std::make_shared<RandomBoard>();
    theBoard->createBoard(seed, fname);
    return theBoard;
}
