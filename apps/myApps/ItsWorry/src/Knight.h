#pragma once

#include "Man.h"

class Knight : public Man
{
public:
	void updatePossibleMoves();
	int getTypeId() { return TYPE_KNIGHT; }
};

/* EOF */