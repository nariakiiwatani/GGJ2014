#include "King.h"
#include "Board.h"

void King::updatePossibleMoves()
{
	possible_move_.clear();
	int x = move_history_.back().first;
	int y = move_history_.back().second;
	checkOne(x,y, -1,-1);
	checkOne(x,y, -1, 0);
	checkOne(x,y, -1, 1);
	checkOne(x,y,  0,-1);
	checkOne(x,y,  0, 1);
	checkOne(x,y,  1,-1);
	checkOne(x,y,  1, 0);
	checkOne(x,y,  1, 1);
}

/* EOF */