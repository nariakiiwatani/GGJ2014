#include "Bishop.h"
#include "Board.h"

bool Bishop::canMove(int x, int y, int move_x, int move_y)
{
	return abs(move_x)==abs(move_y);
}

void Bishop::updatePossibleMoves()
{
	possible_move_.clear();
	int x = move_history_.back().first;
	int y = move_history_.back().second;
	checkLine(x,y, 1,-1);
	checkLine(x,y, 1, 1);
	checkLine(x,y,-1,-1);
	checkLine(x,y,-1, 1);
}

/* EOF */