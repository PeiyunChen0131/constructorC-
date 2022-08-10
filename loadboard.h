#ifndef _LOADBOARD_H_
#define _LOADBOARD_H_
#include <string>
#include "board.h"

class LoadBoard : public Board {
  public:
    virtual void createBoard(unsigned seed, std::string fname) override;
};

#endif
