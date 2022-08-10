#ifndef _LOADLEVEL_H_
#define _LOADLEVEL_H_
#include <memory>
#include "level.h"
#include "board.h"

class LoadLevel : public Level {
  public:
    virtual std::shared_ptr<Board> getBoard(unsigned seed, std::string fname) override;
};

#endif
