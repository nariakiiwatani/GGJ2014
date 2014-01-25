#pragma once

#include "Man.h"

class Pawn : public Man
{
public:
	void updatePossibleMoves();
	bool isJump();
	int getTypeId() { return TYPE_PAWN; }
private:
	bool isPromoted();
};

/* EOF */