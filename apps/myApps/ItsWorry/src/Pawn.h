#pragma once

#include "Man.h"

class Pawn : public Man
{
public:
	void updatePossibleMoves();
	bool isJump();
	void reset();
	int getTypeId() { return TYPE_PAWN; }
	bool canMove(int x, int y, int move_x, int move_y);
	void setPromoted(bool set) { is_promoted_ = set; }
private:
	bool isPromoted() { return is_promoted_; }
	bool is_promoted_;
};

/* EOF */