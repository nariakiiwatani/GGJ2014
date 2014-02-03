#pragma once

#include "Man.h"

class Rook : public Man
{
public:
	void updatePossibleMoves();
	int getTypeId() { return TYPE_ROOK; }
	bool canMove(int x, int y, int move_x, int move_y);
};

/* EOF */