#pragma once

#include "Man.h"

class Rook : public Man
{
public:
	Rook() {
		ofLoadImage(texture_, "rook.png");
	}
	void updatePossibleMoves();
	int getTypeId() { return TYPE_ROOK; }
};

/* EOF */