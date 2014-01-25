#pragma once

#include "Man.h"

class King : public Man
{
public:
	King() {
		ofLoadImage(texture_, "king.png");
	}
	void updatePossibleMoves();
	int getTypeId() { return TYPE_KING; }
};

/* EOF */