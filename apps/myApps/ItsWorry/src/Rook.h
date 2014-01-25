#pragma once

#include "Man.h"

class Rook : public Man
{
public:
	Rook() {
		ofLoadImage(texture_, "rook.png");
	}
	void updatePossibleMoves();
};

/* EOF */