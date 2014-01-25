#pragma once

#include "Man.h"

class Bishop : public Man
{
public:
	Bishop() {
		ofLoadImage(texture_, "bishop.png");
	}
	void updatePossibleMoves();
};

/* EOF */