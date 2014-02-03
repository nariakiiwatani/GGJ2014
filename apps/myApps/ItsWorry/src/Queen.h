#pragma once

#include "Man.h"

class Queen : public Man
{
public:
	void updatePossibleMoves();
	int getTypeId() { return TYPE_QUEEN; }
	bool canMove(int x, int y, int move_x, int move_y);
};

/* EOF */