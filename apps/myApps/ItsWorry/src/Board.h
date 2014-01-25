#pragma once

#include "Man.h"

class Board
{
public:
	static const int GRID_X = 8;
	static const int GRID_Y = 8;
	Board();
	~Board();
	void clear();
	void reset();
	void setMan(int x, int y, int player, int id);
	void draw(float x, float y, float w, float h);
	void drawGrid(float x, float y, float w, float h);
	void drawForPlayer(int p, float x, float y, float w, float h);
	void drawLastMoved(int p, float x, float y, float w, float h);
private:
	Man *board_[GRID_X][GRID_Y];
	vector<Man*> man_[2];
	Man *last_moved_;
};

/* EOF */