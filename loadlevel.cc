#include <memory>
#include "loadlevel.h"
#include "loadboard.h"

std::shared_ptr<Board> LoadLevel::getBoard(unsigned seed, std::string fname) {
    std::shared_ptr<Board> theBoard = std::make_shared<LoadBoard>();
    theBoard->createBoard(seed, fname);
    return theBoard;
}
