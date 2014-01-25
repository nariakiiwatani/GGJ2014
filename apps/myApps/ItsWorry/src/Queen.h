#pragma once

#include "Man.h"

class Queen : public Man
{
public:
	void updatePossibleMoves();
	int getTypeId() { return TYPE_QUEEN; }
};

/* EOF */