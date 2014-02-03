#pragma once

#include "Man.h"

class King : public Man
{
public:
	void updatePossibleMoves();
	int getTypeId() { return TYPE_KING; }
	bool canMove(int x, int y, int move_x, int move_y);
};

/* EOF */