#pragma once

#include "ofImage.h"

typedef pair<int,int> Pos;
class Board;


enum {
	TYPE_PAWN,
	TYPE_ROOK,
	TYPE_KNIGHT,
	TYPE_BISHOP,
	TYPE_KING,
	TYPE_QUEEN,
	TYPE_NUM
};

class Man
{
public:
	static void loadTexture();
	virtual ~Man(){}
	virtual int getTypeId()=0;
	void setBoard(Board *board) { board_=board; }
	virtual void reset();
	bool isMoved() { return is_moved_; }
	void setMoved(bool set) { is_moved_=set; }
	void setSide(int player) { player_=player; }
	int getSide() { return player_; }
	void draw(float x, float y, float w, float h);
	virtual void updatePossibleMoves()=0;
	virtual bool canMove(int x, int y, int move_x, int move_y)=0;
protected:
	void checkLine(int from_x, int from_y, int move_x, int move_y);
	void checkOne(int from_x, int from_y, int move_x, int move_y);
protected:
	Board *board_;
	int player_;
	vector<Pos> possible_move_;
	vector<Pos> move_history_;
	bool is_moved_;
};

/* EOF */