#ifndef _RANDOMLEVEL_H_
#define _RANDOMLEVEL_H_
#include <memory>
#include "level.h"
#include "board.h"

class RandomLevel : public Level {
  public:
    virtual std::shared_ptr<Board> getBoard(unsigned seed, std::string fname) override;
};

#endif
