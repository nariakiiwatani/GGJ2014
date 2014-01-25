#pragma once

#include "ofTexture.h"
#include "ofImage.h"


typedef pair<int,int> Pos;
class Board;


enum {
	TYPE_PAWN,
	TYPE_ROOK,
	TYPE_KNIGHT,
	TYPE_BISHOP,
	TYPE_QUEEN,
	TYPE_KING
};

class Man
{
public:
	Man(){}
	virtual ~Man(){}
	virtual int getTypeId()=0;
	void setBoard(Board *board) { board_=board; }
	bool isLastMoveSafe();
	void reset();
	bool isMoved();
	bool isPosSet();
	void setPos(int x, int y);
	void moveTo(int x, int y);
	Pos getPos() { return move_history_.back(); }
	void setSide(int player) { player_=player; }
	int getSide() { return player_; }
	void draw(float x, float y, float w, float h);
	virtual void updatePossibleMoves()=0;
protected:
	void checkLine(int from_x, int from_y, int move_x, int move_y);
	void checkOne(int from_x, int from_y, int move_x, int move_y);
protected:
	Board *board_;
	ofTexture texture_;
	int player_;
	Pos initial_pos_;
	Pos current_pos_;
	vector<Pos> possible_move_;
	vector<Pos> move_history_;
};

/* EOF */