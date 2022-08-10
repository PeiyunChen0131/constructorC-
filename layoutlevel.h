#ifndef _LAYOUTLEVEL_H_
#define _LAYOUTLEVEL_H_
#include <memory>
#include "level.h"
#include "board.h"

class LayoutLevel : public Level {
  public:
    virtual std::shared_ptr<Board> getBoard(unsigned seed, std::string fname) override;
};

#endif
