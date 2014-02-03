#pragma once

#include "Man.h"

struct Diff {
	Diff(){
		from.x =from.y = to.x = to.y = -1;
	}
	struct {
		int x,y;
	} from, to;
};
typedef map<Man*,Diff> BoardDiff;

class Board
{
public:
	static const int GRID_X = 8;
	static const int GRID_Y = 8;
	Board();
	~Board();
	void clear();
	BoardDiff getDiff(Board *board);
	void merge(BoardDiff& diff);
	bool isLastMoveValid();
	bool isMoved() { return is_moved_; }
	void setMoved(bool set) { is_moved_=set; }
	void removeLast();
	void updatePromotion();
	void set(int x, int y, Man *man);
	void set(Board *board);
	void draw(float x, float y, float w, float h);
	void drawGrid(float x, float y, float w, float h);
	void drawForPlayer(int p, float x, float y, float w, float h);
	
	bool isInBounds(int x, int y);
	Man* get(int x, int y);
	
protected:
	Man *board_[GRID_X][GRID_Y];
	Man *last_moved_;
	bool is_last_move_valid_;
	bool is_moved_;
};

class StableBoard : public Board
{
public:
	void merge(Board *board);
private:
	static const int STABLE_THRESHOLD = 15;
	Man *cache_[GRID_X][GRID_Y];
	int stable_count_[GRID_X][GRID_Y];
};

/* EOF */