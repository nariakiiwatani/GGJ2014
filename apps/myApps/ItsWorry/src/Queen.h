#pragma once

#include "Man.h"

class Queen : public Man
{
public:
	Queen() {
		ofLoadImage(texture_, "queen.png");
	}
	void updatePossibleMoves();
	int getTypeId() { return TYPE_QUEEN; }
};

/* EOF */