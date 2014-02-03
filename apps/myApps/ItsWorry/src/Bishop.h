#pragma once

#include "Man.h"

class Bishop : public Man
{
public:
	void updatePossibleMoves();
	int getTypeId() { return TYPE_BISHOP; }
	bool canMove(int x, int y, int move_x, int move_y);
};

/* EOF */