#include "Knight.h"
#include "Board.h"

bool Knight::canMove(int x, int y, int move_x, int move_y)
{
	return (abs(move_x)==1&&abs(move_y)==2) || (abs(move_x)==2&&abs(move_y)==1);
}

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