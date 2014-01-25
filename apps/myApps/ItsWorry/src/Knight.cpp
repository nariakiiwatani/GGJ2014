#include "Knight.h"
#include "Board.h"

void Knight::updatePossibleMoves()
{
	possible_move_.clear();
	int x = move_history_.back().first;
	int y = move_history_.back().second;
	checkOne(x,y, -2,-1);
	checkOne(x,y, -2, 1);
	checkOne(x,y, -1, 2);
	checkOne(x,y, -1,-2);
	checkOne(x,y,  1, 2);
	checkOne(x,y,  1,-2);
	checkOne(x,y,  2,-1);
	checkOne(x,y,  2, 1);
}

/* EOF */