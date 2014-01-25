#pragma once

#include "Man.h"

class Bishop : public Man
{
public:
	Bishop() {
		ofLoadImage(texture_, "bishop.png");
	}
	void updatePossibleMoves();
	int getTypeId() { return TYPE_BISHOP; }
};

/* EOF */