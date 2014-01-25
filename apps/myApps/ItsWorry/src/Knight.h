#pragma once

#include "Man.h"

class Knight : public Man
{
public:
	Knight() {
		ofLoadImage(texture_, "knight.png");
	}
	void updatePossibleMoves();
	int getTypeId() { return TYPE_KNIGHT; }
};

/* EOF */