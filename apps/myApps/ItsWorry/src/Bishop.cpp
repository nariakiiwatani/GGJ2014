#include "Bishop.h"
#include "Board.h"

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