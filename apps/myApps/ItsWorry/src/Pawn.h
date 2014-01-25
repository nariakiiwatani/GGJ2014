#pragma once

#include "Man.h"

class Pawn : public Man
{
public:
	Pawn() {
		ofLoadImage(texture_, "pawn.png");
	}
	void updatePossibleMoves();
	bool isJump();
	int getTypeId() { return TYPE_PAWN; }
private:
	bool isPromoted();
};

/* EOF */