#include <memory>
#include "layoutlevel.h"
#include "layoutboard.h"

std::shared_ptr<Board> LayoutLevel::getBoard(unsigned seed, std::string fname) {
    std::shared_ptr<Board> theBoard = std::make_shared<LayoutBoard>();
    theBoard->createBoard(seed, fname);
    return theBoard;
}
