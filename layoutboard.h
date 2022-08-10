#ifndef _LAYOUTBOARD_H_
#define _LAYOUTBOARD_H_
#include <string>
#include "board.h"

class LayoutBoard : public Board {
  public:
    virtual void createBoard(unsigned seed, std::string fname) override;
};

#endif
