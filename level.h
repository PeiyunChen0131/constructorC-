#ifndef _LEVEL_H_
#define _LEVEL_H_
#include <memory>
#include "board.h"

class Level {
  public:
    virtual std::shared_ptr<Board> getBoard(unsigned seed, std::string fname) = 0;
};

#endif
