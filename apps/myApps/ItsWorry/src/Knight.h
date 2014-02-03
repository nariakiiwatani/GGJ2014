#pragma once

#include "Man.h"

class Knight : public Man
{
public:
	void updatePossibleMoves();
	int getTypeId() { return TYPE_KNIGHT; }
	bool canMove(int x, int y, int move_x, int move_y);
};

/* EOF */