#pragma once

#include "Man.h"

class Bishop : public Man
{
public:
	void updatePossibleMoves();
	int getTypeId() { return TYPE_BISHOP; }
};

/* EOF */