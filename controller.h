#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_
#include <memory>
#include "board.h"

class Controller {
	std::shared_ptr<Board> theBoard;
	int whosTurn;

	public:
	void cmdCreateBoard(unsigned seed, std::string fname, int type);

	void cmdStartOfGame();

	void cmdSave();

	bool playGame();
};

#endif
