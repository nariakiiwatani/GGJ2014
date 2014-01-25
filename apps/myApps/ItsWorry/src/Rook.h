#pragma once

#include "Man.h"

class Rook : public Man
{
public:
	void updatePossibleMoves();
	int getTypeId() { return TYPE_ROOK; }
};

/* EOF */