#pragma once

#include "Man.h"

class King : public Man
{
public:
	void updatePossibleMoves();
	int getTypeId() { return TYPE_KING; }
};

/* EOF */