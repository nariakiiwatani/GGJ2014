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
private:
	bool isPromoted();
};

/* EOF */