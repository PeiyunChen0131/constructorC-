#ifndef _RANDOMBOARD_H_
#define _RANDOMBOARD_H_
#include <string>
#include "board.h"

class RandomBoard : public Board {
  public:
    virtual void createBoard(unsigned seed, std::string fname) override;
};

#endif
