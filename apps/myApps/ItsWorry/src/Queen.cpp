#include "Queen.h"
#include "Board.h"

bool Queen::canMove(int x, int y, int move_x, int move_y)
{
	return move_x == 0 || move_y == 0 || abs(move_x)==abs(move_y);
}

void Queen::updatePossibleMoves()
{
	possible_move_.clear();
	int x = move_history_.back().first;
	int y = move_history_.back().second;
	checkLine(x,y, -1,-1);
	checkLine(x,y, -1, 0);
	checkLine(x,y, -1, 1);
	checkLine(x,y,  0,-1);
	checkLine(x,y,  0, 1);
	checkLine(x,y,  1,-1);
	checkLine(x,y,  1, 0);
	checkLine(x,y,  1, 1);
}

/* EOF */