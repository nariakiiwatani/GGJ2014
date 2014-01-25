#pragma once

#include "Man.h"

class Knight : public Man
{
public:
	Knight() {
		ofLoadImage(texture_, "knight.png");
	}
	void updatePossibleMoves();
};

/* EOF */