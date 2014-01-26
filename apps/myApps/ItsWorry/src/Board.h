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
	void prepare();
	void update();
	void setMan(int x, int y, int player, int id);
	void draw(float x, float y, float w, float h);
	void drawGrid(float x, float y, float w, float h);
	void drawForPlayer(int p, float x, float y, float w, float h);
	void drawLastMoved(int p, float x, float y, float w, float h);
	
	bool doubt();
	
	bool isInBounds(int x, int y);
	Man* getMan(int x, int y);
	Man* getManPrev(int x, int y);
	vector<Pos> getMovablePos(Man *man);
	Man* getLastMoved() { return last_moved_; }
	bool isMovedFrame();
	
private:
	Man *board_[GRID_X][GRID_Y];
	Man *board_prev_[GRID_X][GRID_Y];
	Man *board_last_[GRID_X][GRID_Y];
	int board_stable_count_[GRID_X][GRID_Y];
	vector<Man*> man_[2];
	Man *last_moved_;
	int doubt_frame_;
	bool moved_frame_;
};

/* EOF */